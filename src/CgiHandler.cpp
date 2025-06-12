#include "../includes/CgiHandler.hpp"

// constructors/destructors
CgiHandler::CgiHandler(const std::string &path, const std::map<std::string, std::string> &env, const std::string &body) : _scriptPath(path), _env(env), _body(body) {}

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
	}
	return *this;
}

CgiHandler::~CgiHandler() {}

std::string CgiHandler::execute()
{
	int pipeIn[2];
	int pipeOut[2];
	pipe(pipeIn);
	pipe(pipeOut);
	std::string result;

	pid_t pid = fork();
	if (pid == 0)
	{
		dup2(pipeIn[0], STDIN_FILENO);
		dup2(pipeOut[1], STDOUT_FILENO);
		close(pipeIn[1]);
		close(pipeOut[0]);
		char *argv[] = {strdup(_scriptPath.c_str()), NULL};
		char **envp = new char*[_env.size() + 1];
		int i = 0;
		for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); ++it)
		{
			std::string kv = it->first + "=" + it->second;
			envp[i] = strdup(kv.c_str());
			i++;
		}
		envp[i] = NULL;
		execve(_scriptPath.c_str(), argv, envp);
		exit(1);
	}
	else
	{
		close(pipeIn[0]);
		close(pipeOut[1]);
		if (!_body.empty())
		{
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
	}
	return result;
}