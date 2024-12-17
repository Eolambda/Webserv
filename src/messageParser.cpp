#include "../inc/messageParser.hpp"

Message	messageParser::parseMessage(std::string flux)
{
	Message		message;
	std::string	line;
	std::string	key;
	std::string	value;

	std::istringstream iss(flux);
	std::getline(iss, line);
	if (line.empty())
		throw std::runtime_error("Error: empty request line");
	parseRequestLine(line, message);

	//Parse headers
	while (std::getline(iss, line))
	{
		if (line.empty())
			break;
		if (line.find(":") == std::string::npos)
			throw std::runtime_error("Error: invalid header");
		key = line.substr(0, line.find_first_of(":"));
		key = Utils::trimSpaces(key);
		value = line.substr(line.find_first_of(":") + 1);
		value = Utils::trimSpaces(value);
		message.addHeader(key, value);
	}
	message.setBody(flux.substr(flux.find("\r\n\r\n") + 4));
	return (message);
}

//Parse first line, that is going to be the request line in the form method SP request-target SP HTTP-version
void	messageParser::parseRequestLine(std::string line, Message &message)
{
	size_t		pos;
	std::string method;
	std::string request_target;
	std::string http_version;

	if ((pos = line.find(" ")) == std::string::npos)
		throw std::runtime_error("Error: invalid request line");
	method = line.substr(0, pos);
	method = Utils::trimSpaces(method);
	line.erase(0, pos + 1);
	if ((pos = line.find(" ")) == std::string::npos)
		throw std::runtime_error("Error: invalid request line");
	request_target = line.substr(0, pos);
	request_target = Utils::trimSpaces(request_target);
	line.erase(0, pos + 1);
	http_version = line;
	http_version = Utils::trimSpaces(http_version);

	//Have to verify here that informations are correct
	message.setType(REQUEST);
	message.setMethod(method);
	message.setRequestTarget(request_target);
	message.setHttpVersion(http_version);
}