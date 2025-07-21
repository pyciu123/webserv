#include "../includes/CgiHandler.hpp"
#include <cstdlib> // dla free

// constructors/destructors
CgiHandler::CgiHandler(const std::string &path, const std::map<std::string, std::string> &env, const std::string &body, const std::string interpreter) : _scriptPath(path), _env(env), _body(body), _interpreter(interpreter) {}

CgiHandler::CgiHandler(const CgiHandler &copy)
{
    *this = copy;
}

CgiHandler &CgiHandler::operator=(const CgiHandler &other)
{
    if (this != &other)
    {
        _scriptPath = other._scriptPath;
        _env = other._env;
        _body = other._body;
        _interpreter = other._interpreter;
    }
    return *this;
}

CgiHandler::~CgiHandler() {}

std::string CgiHandler::execute()
{
    int pipeIn[2];
    int pipeOut[2];
    if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1) {
        std::cerr << "Pipe creation failed: " << strerror(errno) << std::endl;
        return "";
    }

    std::string result;
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(pipeIn[0], STDIN_FILENO);
        dup2(pipeOut[1], STDOUT_FILENO);
        close(pipeIn[1]);
        close(pipeOut[0]);

        char *argv[] = {strdup(_interpreter.c_str()), strdup(_scriptPath.c_str()), NULL};
        char **envp = new char*[_env.size() + 1];
        int i = 0;
        for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); ++it)
        {
            std::string kv = it->first + "=" + it->second;
            envp[i] = strdup(kv.c_str());
            std::cerr << "Env: " << kv << std::endl;
            i++;
        }
        envp[i] = NULL;

        std::cerr << "Executing: " << _interpreter << " " << _scriptPath << std::endl;
        if (execve(_interpreter.c_str(), argv, envp) == -1) {
            std::cerr << "execve failed: " << strerror(errno) << std::endl;
            for (i = 0; envp[i]; ++i) free(envp[i]);
            delete[] envp;
            free(argv[0]);
            free(argv[1]);
            exit(1);
        }
    }
    else if (pid > 0)
    {
        close(pipeIn[0]);
        close(pipeOut[1]);
        if (!_body.empty())
        {
            std::cerr << "Writing body to CGI: " << _body << std::endl;
            write(pipeIn[1], _body.c_str(), _body.size());
        }
        close(pipeIn[1]);

        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(pipeOut[0], buffer, sizeof(buffer))) > 0)
            result.append(buffer, bytesRead);
        close(pipeOut[0]);

        int status;
        waitpid(pid, &status, 0);
        std::cerr << "CGI result: " << result << std::endl;
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            std::cerr << "CGI exited with error status: " << WEXITSTATUS(status) << std::endl;
        }
    }
    else
    {
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
        close(pipeIn[0]);
        close(pipeIn[1]);
        close(pipeOut[0]);
        close(pipeOut[1]);
        return "";
    }
    return result;
}