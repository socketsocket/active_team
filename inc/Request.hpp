#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
public:
	enum Method { GET, POST, DELETE };

	Request();
	Request(const Request& req);
	~Request();

	Request& operator= (const Request &req);

private:
	int		status;
	Method	method;
	std::string uri;
	std::string http_version;
	std::map<std::string, std::string> headers;  // 헤더 중복 허용 안함, 중복시 400 Bad Request, Connection: close
	std::string body;
};

Request::Request()
{
		// int client_socket;
		// 			// accept(listen_fd,(struct sockaddr *)&client_addr, &addrlen);
		// 			client_socket = accept(server_socket, NULL, NULL); // client address가 필요 없어서 NULL!
		// 			if (client_socket == -1)
		// 				exit_with_perror("accept() error");
		// 			std::cout << "accept new client: " << client_socket << std::endl;
		// 			fcntl(client_socket, F_SETFL, O_NONBLOCK);

		// 			EV_SET(&temp_event, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
		// 			change_list.push_back(temp_event);
		// 			EV_SET(&temp_event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		// 			change_list.push_back(temp_event);

		// 			client_data[client_socket] = "";
}

#endif