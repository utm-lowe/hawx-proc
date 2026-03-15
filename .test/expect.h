/**
 * @file expect.h
 * @author Robert Lowe <pngwen@acm.org>
 * @brief A very simplified version of expect for C++
 * @version 0.1
 * @date 2025-01-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef EXPECT_H
#define EXPECT_H
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <chrono>
#include <thread>

/**
 * @brief An expect object interacts with a program either via a stream or by spawning it.
 * 
 */
class Expect {
public:
    Expect() {
        in = stdin;
        out = stdout;
        pid = 0;
        mstimeout = 10000;
        timerStart();
    }

    /**
     * @brief Opens an interactive command for processing.
     * 
     * @param command 
     */
    virtual void spawn(const std::string &command) {
        int inPipe[2];
        int outPipe[2];

        // create the pipes for input and output
        pipe(inPipe);
        pipe(outPipe);

        // for the process to interact with the pipes
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            close(inPipe[0]);
            close(outPipe[1]);

            // redirect stdin and stdout
            dup2(outPipe[0], fileno(stdin));
            dup2(inPipe[1], fileno(stdout));

            // close the pipes
            close(outPipe[0]);
            close(inPipe[1]);

            // execute the command
            execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
        } else {
            this->pid = pid;

            // parent process
            close(outPipe[0]);
            close(inPipe[1]);

            // put the pipes into non-blocking mode
            unblock(outPipe[1]);
            unblock(inPipe[0]);

            // set the input and output streams
            in = fdopen(inPipe[0], "r");
            out = fdopen(outPipe[1], "w");
        }
    }

    /**
     * @brief Expect matchers for testing when we get matching input.
     * 
     */
    class Matcher {
    public:
        virtual ~Matcher() {}
        virtual bool match(char c) = 0;
    };

    class StringMatcher : public Matcher {
    public:
        StringMatcher(const std::string &str) : str(str), index(0) {}
        virtual bool match(char c) {
            if (c == str[index]) {
                index++;
                if (index == str.size()) {
                    index = 0;
                    return true;
                }
            } else {
                index = 0;
            }
            return false;
        }
    private: 
        std::string str;
        size_t index;
    };

    class StringNoCaseMatcher : public Matcher {
    public:
        StringNoCaseMatcher(const std::string &str) : str(str), index(0) {}
        virtual bool match(char c) {
            if (tolower(c) == tolower(str[index])) {
                index++;
                if (index == str.size()) {
                    index = 0;
                    return true;
                }
            } else {
                index = 0;
            }
            return false;
        }
    private:
        std::string str;
        size_t index;
    };

    /**
     * @brief Run until the match is made or until we timeout.
     * 
     * @param matcher 
     * @return true Input was matched before time expired
     * @return false Input was not matched before time expired
     */
    virtual bool expect(Matcher &matcher) {
        char c;
        timerStart();
        while (!feof(in)) {
            if(timerExpired()) break;
            c = getchar();
            if(c<0) continue;

            if (matcher.match(c)) {
                return true;
            }
        }
        return false;
    }


    /** 
     * @brief Expect a string to be received from the program.
     * 
     * @param str The string to expect
     * @param ignore_case Ignore case when matching
     * @return true The string was received
     * @return false The string was not received
     */
    virtual bool expect(const std::string &str, bool ignore_case) {
        if (ignore_case) {
            StringNoCaseMatcher matcher(str);
            return expect(matcher);
        } else {
            StringMatcher matcher(str);
            return expect(matcher);
        }
    }

    /**
     * @brief Expect a string to be received from the program.
     * 
     * @param str The string to expect
     * @return true The string was received
     * @return false The string was not received
     */
    virtual bool expect(const std::string &str) {
        return expect(str, false);
    }

    /**
     * @brief Send a string to the program.
     * 
     * @param str The string to send
     */
    virtual void send(const std::string &str) {
        fprintf(out, "%s", str.c_str());
        fprintf(stdout, "%s", str.c_str());
        fflush(out);
        fflush(stdout);
    }

    /**
     * @brief Set the timeout in milliseconds. 
     * 
     * @param mstimeout 
     */
    virtual void setTimeout(int mstimeout) {
        this->mstimeout = mstimeout;
    }

    /**
     * @brief Get the timeout in milliseconds.
     * 
     * @return int 
     */
    virtual int getTimeout() {
        return mstimeout;
    }

    /**
     * @brief Read input until the timeout expires.
     * 
     */
    virtual void timeout() {
        timerStart();
        while (!feof(in)) {
            getchar();
            if(timerExpired()) break;
        }
    }

    virtual void killProc() {
        if (pid > 0) {
            kill(pid, SIGKILL);
            wait(NULL);
        }
    }   

    virtual std::string getline() {
        std::string line;
        char c;

        timerStart();
        do {
            if(timerExpired()) break;
            c = getchar();
            if(c >=0 and c != '\n') {
                line += c;
            }
        } while(c != '\n');

        return line;
    }
private:
    /**
     * @brief Activate non-blocking mode on a file descriptor.
     * 
     * @param fd 
     */
    void unblock(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    /**
     * @brief Start the timeout timer
     * 
     */
    void timerStart() {
        start = std::chrono::system_clock::now();
    }

    /**
     * @brief Check if the timeout has expired.
     * 
     * @return true The timeout has expired
     * @return false The timeout has not expired
     */
    bool timerExpired() {
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
        return elapsed_seconds.count() * 1000 > mstimeout;
    }

    char getchar()
    {
        char c = fgetc(in);
        if(c >= 0) {
            putchar(c);
        }
        return c;
    }

    FILE *in;
    FILE *out;
    pid_t pid;
    int mstimeout;
    std::chrono::time_point<std::chrono::system_clock> start;
};
#endif