#include "ServerManager.hpp"

static std::string
	pop_front(std::deque<std::string> *config_deque)
{
	if (config_deque->empty() == true)
		throw ServerManager::parseException("Error occured: unexcepted EOF");

	std::string	return_value = config_deque->front();
	config_deque->pop_front();
	return (return_value);
}

template <typename InputIt>
static void
	string_tolower(InputIt first, InputIt last)
{
	for (InputIt itr = first; itr != last; ++itr)
		*itr= std::tolower(*itr);
}

ServerManager::ServerManager(std::string config_path)
{
	std::ifstream	config_file(config_path);
	std::deque<std::string>	config_deque;
	std::string	word;

	while (config_file >> word)
	{
		string_tolower(word.begin(), word.end());
		if (word[0] == '#')
			std::getline(config_file, word);
		else
			config_deque.push_back(word);
	}
	while (config_deque.empty() == false)
	{
		if (config_deque.front() == "server")
		{
			config_deque.pop_front();
			if (config_deque.front() != "{")
				throw (parseException("Server Parsing Error : no exist '{'"));
			else
			{
				config_deque.pop_front();
				servers.push_back(parseServer(&config_deque));
			}
		}
		else
			throw (parseException("No Server Block"));
	}
}

Server
	*ServerManager::parseServer(std::deque<std::string> *config_deque)
{
	// Server	*serv = new Server();

	std::string					server_name = "";
	std::queue<unsigned int>	ports;
	std::string					word;

	bool	exist_body_limit = false;
	bool	exist_auto_index = false;

	while ((word = pop_front(config_deque)) != "}")
	{
		if (word == "listen")
			std::cout << "client_body_limit = " << std::stoul(pop_front(config_deque)) << std::endl;
			// ports.push(std::stoul(pop_front(config_deque)));
		else if (word == "server_name")
			std::cout << "server_name = " << pop_front(config_deque) << std::endl;
			// server_name = pop_front(config_deque);
		else if (word == "client_body_limit")
		{
			std::cout << "client_body_limit = " << std::stoul(pop_front(config_deque)) << std::endl;

			if (exist_body_limit == true)
				throw ServerManager::parseException("Server Parsing Error: double client_body_limit input");
			exist_body_limit = true;
			// serv->setBodyLimit(std::stoul(pop_front(config_deque)));
		}
		else if (word == "auto_index")
		{
			std::cout << "auto_index = " << pop_front(config_deque) << std::endl;
			
			if (exist_auto_index == true)
				throw ServerManager::parseException("Server Parsing Error: double auto_index input");
			exist_auto_index = true;
			// serv->setAutoindex(pop_front(config_deque));
		}
		else if (word == "error_page")
		{
			std::cout << "error_page = " << pop_front(config_deque) << ", " << pop_front(config_deque) << std::endl;

			// int			error_code = std::stoi(pop_front(config_deque));
			// std::string	error_page = pop_front(config_deque);

			// serv->addErrorPage(error_code, error_page);
		}
		else if (word == "location")
		{
			std::string	location_path = pop_front(config_deque);

			std::cout << "location = " << location_path << std::endl;

			// if (config_deque->front() != "{")
				// throw ServerManager::parseException("Location Parsing Error : no exist '{'");
			if (pop_front(config_deque) != "{")
				throw ServerManager::parseException("Location Parsing Error : no exist '{'");
			else
				parseLocation(config_deque, location_path);
				// serv->addLocation(location_path, parseLocation(config_deque, location_path));
		}
		else if (word == "return")
		{
			int			code = std::stoul(pop_front(config_deque));
			std::string	uri = pop_front(config_deque);

			if (uri == ";")
				std::cout << "code = " << code << std::endl;
				// serv->setReturnInfo(code, "");
			else
			{
				std::cout << "code = " << code << "uri = " << uri << std::endl;
				// serv->setReturnInfo(code, uri);
				if (config_deque->front() != ";")
					throw ServerManager::parseException("Location Parsing Error : no exist ';'");
			}
		}
		else
			throw ServerManager::parseException(word + ": unexcepted direction");

		if (word != "location" && pop_front(config_deque) != ";")
			throw ServerManager::parseException("Excepted ';'");
	}

	if (word != "}")
		throw ServerManager::parseException("Server Parsing Error : no exist '}");

	// for (; ports.empty() == false; ports.pop())
		// this->uri_to_server.insert(make_pair(make_pair(server_name, ports.front()), serv));
	// return (serv);
	return (0); // 파싱 테스트용
}

Location
	ServerManager::parseLocation(std::deque<std::string> *config_deque, std::string location_path)
{
	Location	loc(location_path);
	std::string	word;
	bool		exist_auto_index = false;
	
	while ((word = pop_front(config_deque)) != "}")
	{
		if (word == "root")
		{
			std::cout << "root = " << pop_front(config_deque) << std::endl;
			// if (loc.setRoot(pop_front(config_deque)) == FAILED)
				// throw ServerManager::parseException("Location Parse Error: double root setting");
		}
		else if (word == "index")
		{
			std::cout << "index = " << pop_front(config_deque) << std::endl;
			// loc.addIndex(pop_front(config_deque));
		}
		else if (word == "auto_index")
		{
			std::cout << "auto_index = " << pop_front(config_deque) << std::endl;
			// if (exist_auto_index == true)
				// throw ServerManager::parseException("Location Parse Error: double autoindex setting");
			// exist_auto_index = true;
			// loc.setAutoindex(pop_front(config_deque));
		}
		else if (word == "error_page")
		{
			std::cout << "error_page = " << pop_front(config_deque) << ", " << pop_front(config_deque) << std::endl;
			
			// if (loc.addErrorPage(stoi(pop_front(config_deque)), pop_front(config_deque)) == FAILED)
				// throw ServerManager::parseException("Location Parse Error: invalid error page");
		}
		else if (word == "methods_allowed")
		{
			std::cout << "methods_allowed = ";

			std::string	method;
			while ((method = pop_front(config_deque) ) != ";")
			{
				// if (method == "GET")
					// loc.addMethod(Request::GET);
				// else if (method == "POST")
					// loc.addMethod(Request::POST);
				// else if (method == "DELETE")
					// loc.addMethod(Request::DELETE);
				std::cout << method << " ";
			}
			std::cout << std::endl;
		}
		else if (word == "cgi_info")
		{
			// loc.addCGI(pop_front(config_deque), pop_front(config_deque));
			std::cout << "cgi_info = " << pop_front(config_deque) << ", " << pop_front(config_deque) << std::endl;
		}
		else if (word == "return")
		{
			int			code = std::stoul(pop_front(config_deque));
			std::string	uri = pop_front(config_deque);

			if (uri == ";")
				std::cout << "code = " << code << std::endl;
				// serv->setReturnInfo(code, "");
			else
			{
				std::cout << "code = " << code << "uri = " << uri << std::endl;
				// serv->setReturnInfo(code, uri);
				if (config_deque->front() != ";")
					throw ServerManager::parseException("Location Parsing Error : no exist ';'");
			}
		}
		else
			throw ServerManager::parseException(word + ": unexcepted direction");

		if (word != "methods_allowed" && pop_front(config_deque) != ";")
			throw ServerManager::parseException("Excepted ';'");
	}

	if (word != "}")
		throw ServerManager::parseException("Server Parsing Error : no exist '}");
	return (loc);
}

void exit_with_perror(const std::string message)
{
	perror(message.c_str());
	exit(1);
}

static void
	print_open_status(std::string server_name, unsigned int server_port)
{
	std::cout << server_name << ":" << server_port << " started" << std::endl;
	return ;
}

int	ServerManager::openPort()
{
	typedef	std::map<std::pair<std::string, unsigned int>, Server *>::iterator	portmap_iterator;

	std::set<unsigned int>		open_port;

	if ((this->kq = kqueue()) == -1)
		return (perror("kqueue error"), 1);

	for (portmap_iterator itr = uri_to_server.begin(); itr != uri_to_server.end(); itr++)
	{
		const unsigned int	port = itr->first.second;

		if (open_port.find(port) != open_port.end())
		{
			print_open_status(itr->first.first, port);
			continue ;
		}

		int					server_socket;
		struct sockaddr_in	server_addr;

		server_socket = socket(PF_INET, SOCK_STREAM, 0);
		if (server_socket == -1)
			return (perror("socket error"), 1);

		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(port);

		if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
			return (perror("socket error"), 1);

		if (listen(server_socket, 5) == -1)
			return (perror("socket error"), 1);

		fcntl(server_socket, F_SETFL, O_NONBLOCK);

		struct kevent	event;
		EV_SET(&event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(kq, &event, 1, NULL, 0, NULL);

		open_port.insert(port);
		server_socket_to_port.insert(std::make_pair(server_socket, port));

		print_open_status(itr->first.first, port);
	}
}

void
	ServerManager::deleteClient(int client_socket)
{
	delete client_socket_to_client.find(client_socket)->second;
	client_socket_to_client.erase(client_socket);
}


int	ServerManager::monitorEvent()
{
	int							num_events;
	struct kevent				*curr_event;
	std::vector<struct kevent>	change_list;
	struct kevent				event_list[EVENTSIZE];
	struct kevent				temp_event;           // client read/write 등록용

	while (true)
	{
		// if (change_list.size() == 0)
		// 	num_events = kevent(kq, NULL, 0, event_list, EVENTSIZE, NULL);
		// else
		num_events = kevent(kq, &change_list[0], change_list.size(), event_list, EVENTSIZE, NULL);
		if (num_events == -1)
			return (perror("kevent error"), 1);

		change_list.clear();

		// 6
		// 6q 6q 6q 6q 6q
		//    6q 6q 6q 6q
		//    6q

		// 6q -> client ->req complete->serverManager->server->location -> ".html" "read"

		for (int i = 0; i < num_events; ++i)
		{
			curr_event = &event_list[i];
			if (curr_event->flags & EV_ERROR)
			{
				if (server_socket_to_port.find(curr_event->ident) != server_socket_to_port.end())
					return (perror("server socket error"), 1);
				else
				{
					deleteClient(curr_event->ident);
					return (perror("client socket error"), 1);
				}
			}
			else if (curr_event->filter == EVFILT_READ)
			{
				if (server_socket_to_port.find(curr_event->ident) != server_socket_to_port.end())
				{
					//이거 포인터임 주의
					Client *client = new Client(curr_event->ident, server_socket_to_port.find(curr_event->ident)->second);

					client_socket_to_client.insert(std::make_pair(client->getSocketFD(), client));

					EV_SET(&temp_event, client->getSocketFD(), EVFILT_READ, EV_ADD, 0, 0, NULL);
					change_list.push_back(temp_event);
					EV_SET(&temp_event, client->getSocketFD(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
					// EVFILT_WRITE: Takes a file descriptor as the identifier, and returns whenever it is possible to write to the descriptor.
					change_list.push_back(temp_event);
				}
				else if (client_socket_to_client.find(curr_event->ident) != client_data.end())
				{
					Client client = client_socket_to_client.find(curr_event->ident);
					// 클라이언트가 요청을 처리
					// RawRequestReader를 통해
					char	buffer[1024];
					int		n;

					n = read(curr_event->ident, buffer, sizeof(buffer) - 1);
					if (n == -1)
					{
						std::cerr << "read error" << std::endl;
						continue;
					}
					else if (n == 0)
					{
						std::cout << "client disconnected" << std::endl;
						client_data.erase(curr_event->ident);
						close(curr_event->ident);
					}
					else
					{
						buffer[n] = '\0';
						client_data[curr_event->ident] += buffer;
					}
					//raw_request 호출
/*----------------------------- 여기부터 진행 issue #5, 요청이 완료되었다는 전제 -----------------------------------------*/
					if (client.getStatus() == REQUEST_COMPLETE)
						// prepareResponse();
						// resource_fd = 
						client.prepareResponse();
					if (client.getStatus() == NEED_RESOURCE)
					{
						types[client->dialog->reponse->resource.fd] = make_pair(client_fd, client->dialog->reponse->resource);
						EV_SET(client->reponse->resource.fd);
					}
					if (client.getStatus() == RESPONSE_COMPLETE)
						EV_SET(client write event);
				}
				else if (FDType == RESOURCE)
				{
					// 리소스를 읽고나면 그게 클라이어느 누구껀지 어떻게 알아 ?
					// EV_SET(client_fd);
				}
			}
	//resource 의 상태는 누가 ? response 안에 있는 resource 가
	//리소스의 포인터나 레퍼런스를 서버매니저에 보내고 그 fd 를 가지고 등록
	//서버매니저는 열심히 읽고 다 읽으면 리소스의 형태를 바꾼다
			// else if (curr_event->filter == EVFILT_WRITE)
			// {
			// 	if (client.getStatus == "RESPONSE_COMPLETE")
			// 	{
			// 		int	n;
			// 		if((n = write(curr_event->ident, client_data[curr_event->ident].c_str(), client_data[curr_event->ident].length())) == -1)
			// 			std::cerr << "write error" << '\n';
			// 		client_data[curr_event->ident].clear();
			// 	}
			// }
		}
	}
	return (0);
}

// 클라이언트에서 내 프로세스처리클래스에서 응답을 준비시키고
// 그 안에서 서버가 makeResponse 를 실행시키고
// 그 과정에서 리스폰스 클래스에 있는 addHeader 함수 등을 실행시켜 필요한 정보를 담는다


void 	Client::prepareResponse() //maybe Client::ResourceProcesser::prepareResponse()
{
	// 최종적으로 client.Response 에 바디 전 내용이 담겨있으면 됨
	// 지정해 줄 리소스가 없으면 내가 make 해서 body 에도 담아야 함!

	//먼저 포트매니저의 서버의 로케이션이랑 클라이언트랑 매칭시키는 과정이 필요
	//클라이언트가 해당 포트매니저를 레퍼런스로 가지고 있음 나는 호스트네임을 통해 서버를 매칭시켜주면됨
	//포트매니저를 통해 서버를 불러오고 그 서버를 통해 요청 uri에 맞는 로케이션을 불러오고
	Server		server = this->portmanager.getServer(this->request.getHeaderValue("HOST"));
	Location	location = server.getLocation(this->request.getUri());


	// 405 - Allowed Method
	if (location.getMethodAllowed().empty() ||
			std::find(location.getMethodAllowed().begin(), location.getMethodAllowed().end(), request.getMethod()) == location.getMethodAllowed().end())
	{
		this->response = server.makeErrorResponse(server, location, 405);
		// pingpong->getResponse() = server.makeErrorResponse(server, location, 405); 나중에 바꿔야함!!
		// client.getResponse().addHeader()
		// return (405);
	}
	
	// CLIENT_BODY_LIMIT
	if (request.getBody().length() > server.getBodyLimit())
		this->response = server.makeErrorResponse(server, location, 413);

	// Server Block return
	if (server.getReturnCode() != 0)
		this->response = server.makeReturnResponse(server.getReturnCode());

	// response
	if (1)//isCgiRequest())
	{}
	else
	{
		//일단 요청uri 랑 매칭되는 로케이션 path 를 찾아서 root 로 바꿔줘야 함
		std::string resource_path = this->request.getUri();
		resource_path.replace(0, location.getPath().length(), location.getRoot());
		
		if (request.getMethod() == Request::GET) // enum
			this->response = server.makeGETResponse(location, resource_path);
		else if (request.getMethod() == Request::POST)
			this->response = server.makePOSTResponse(location, resource_path);
		else if (request.getMethod() == Request::DELETE)
			this->response = server.makeDELETEResponse(location, resource_path);
		this->status = NEED_RESOURCE;
	}
}

std::string Server::makeHTMLPage(std::string content)
{
	std::string html;

	html += "<html>\r\n";

	html += "<head>\r\n";
	html += "</head>\r\n";

	html += "<body>\r\n";
	html += "<hr>\r\n";
	html += "<pre>\r\n";
	html += content;
	html += "</pre>\r\n";
	html += "<hr>\r\n";
	html += "</body>\r\n";

	html += "</html>";
	
	return (html);
}

// <html>
// 	<head>
// 		<title>404 Not Found</title>
// 	</head>
// 	<body>
// 		<center>
// 			<h1>404 Not Found</h1>
// 		</center>
// 		<hr>
// 		<center>nginx/1.21.1</center>
// 	</body>
// </html>

std::string 	Server::generateErrorPage(size_t error_code)
{
	std::string html;

	html += "<html>\r\n";

	html += "<head>\r\n";
	html += "<title>" + error_code + " " + this->statusMessage(error_code) + "</head>\r\n";
	html += "</head>\r\n";

	html += "<body>\r\n";
	html += "<center>\r\n";
	html += "<h1>" + error_code + " " + this->statusMessage(error_code) + "</h1";
	html += "</center>\r\n";;
	html += "<hr>\r\n";
	html += "<center> hsonseyu/1.1</center>";
	html += "</body>\r\n";

	html += "</html>";
	
	return (html);
}

Response Server::makeErrorResponse(Server &server, Location &location, size_t error_code)
{
	//에러페이지가 있고 그걸 열어야 하면 kevent 에 resource read event 를 열고
	//없으면 내가 자체 제작 한 후 응답 메시지 완성!

	struct kevent event;

	Response response;
	response.addHeader(std::string("Date"), this->dateHeader());
	response.addHeader(std::string("Server"), "hsonseyu Server");
	response.addHeader(std::string("Content-Type"), this->contentTypeHeader(".html"));

	// 로케이션에 없으먼 -> 서버에 있는지 찾아보고 -> 만들기
	int fd = 0;
	struct stat buf;
	if (location.getErrorPages().count(error_code))
		fd = open(location.getErrorPages()[error_code].c_str(), O_RDONLY);
	else if (server.getErrorPages().count(error_code))
		fd = open(server.getErrorPages()[error_code].c_str(), O_RDONLY);
	else
		response.addBody(this->generateErrorPage(error_code));

	if (fd == -1)
		response.addBody(this->generateErrorPage(error_code));
	else
	{
		response.setResoureFD(fd);

		EV_SET(&event, fd, EVFILT_READ, EV_ADD, NULL, NULL, NULL);
		return (response);
	}

	EV_SET(&event, /*client fd*/, EVFILT_WRITE, EV_ADD, NULL, NULL, NULL);
	return (response);
}

int		Server::checkPath(std::string path)
{
	struct stat buf;

	if (stat(path.c_str(), &buf) == -1)
		return NotFound;
	else
	{
		if (S_ISREG(buf.st_mode))
			return File;
		else if (S_ISDIR(buf.st_mode))
			return Directory;
	}
}

// <html><head><title>Index of /</title></head>
// <body>
// <h1>Index of /</h1><hr><pre>
// <a href="../">../</a>
// <a href="post_test/">post_test/</a>                                         27-Jul-2021 01:45                   -
// <a href="put_test/">put_test/</a>                                          27-Jul-2021 01:41                   -
// <a href="50x.html">50x.html</a>                                           06-Jul-2021 14:59                 494
// <a href="index.html">index.html</a>                                         06-Jul-2021 14:59                 612
// </pre><hr>

// </body></html>

std::string	Server::makeAutoIndexPage(std::string path, std::string uri, Location &location)
{
	DIR *dir;
	struct dirent *dir_read;

	std::string body;
	body += "<html>\r\n";
	body += "<head>\r\n";
	body += "<title>Index of " + uri + "</title>\r\n";
	body += "</head>\r\n";
	body += "<body>\r\n";
	body += "<h1>Index of " + uri + "</h1>\r\n";
	body += "<hr>\r\n";
	body += "<pre>\r\n";
	//<a>
	//이전 함수에서 디렉토리가 안열리면 에러처리를 했으니 여기선 열린 경우만 생각해도 될 듯
	if((dir = opendir(path.c_str())) == 0)
		return (500);
	
	while ((dir_read = readdir(dir)) != 0)
	{
		std::string name = std::string(dirread->d_name);
		if (dir_read->d_type == DT_DIR)
			name += '/';
		body += "<a href=\"" + name + "\">" + name + "</a><br>\n";

		//시간 dd-mm-yyyy hh:mm
		time_t	curr_time = time(NULL);
		struct	tm*	timeinfo;
		char	buffer[64];
		timeinfo = localtime(&curr_time);
		strftime(buffer, 64, "%d-%b-%Y %H:%M", timeinfo);
		body += "                                       " + std::string(buffer);

	}
	closedir(dir);

	body += "</pre>\r\n";
	body += "<hr>\r\n";
	body += "</body>\r\n";
	body += "</html>";

	// 200 ok text/html
	// this->headers.insert(std::pair<std::string, std::string>("Content-Type", "text/html"));
	
	// 리스폰스 완성 나머지 헤더는 돌아가서 ?
	return (body);
}

//스타트라인은 어디서 만들지 ????????????????????????

Response Server::makeGETResponse(Location &location, std::string path)
{
	Response response;
	response.addHeader("Date", this->dateHeader());
	response.addHeader("Server", "hsonseyu Server");

	Resouece resource;

	if (checkPath(path) == Directory)
	{
		if (path[path.length() - 1] != '/')
			path += '/';
		//일단 우선 Index file 들 확인
		//뭐 열었는데 없거나, 디렉토리면 error(404)
		//auto_index 확인
		struct stat buf;
		bool found = false;
		for (std::vector<std::string>::iterator iter = location.getIndex().begin(); iter != location.getIndex().end(); iter++)
		{
			if (stat((path + *iter).c_str(), &buf) == 0)	//여기 때문에 getIndex 에서 const 지움
			{
				found = true;
				path = path + *iter;
				break ;
			}
		}
		if (found == false && location.isAutoIndex() == true)
		{
			response.addHeader("Content-Type", this->contentTypeHeader(".html"));
			response.addBody(this->makeAutoIndexPage(path, uri, location));
			response.addHeader("Content-Length", response.getBody.length());
			//200 ok?
		}
		else
			this->makeErrorResponse(server, location, 404);
	}
	//경로가 특정 파일로 지정됨! 오픈!

	if (int fd = open(path.c_str(), O_RDONLY) == -1)
		this->makeErrorResponse(server, location, 404);

	//리소스 fd 추가
	//뭐랑 뭐랑 가지고 있기로 했더라 ? 어디다가 매칭시켜두지 ? 리스폰스 ? 클라이언트도 알아야했던 것 같은데
	response.setResoureFD(fd);
	struct kevent event;

	EV_SET(&event, fd, EVFILT_READ, EV_ADD, NULL, NULL, NULL);
	return (response);
}

Response Server::makePOSTResponse(Location &location, std::string resource_path)
{
	//POST 는 대부분 cgi 처리를 원함. cgi 가 아닌 서버에서의 POST 의 경우 파일 생성
	Response response;
	Resouece resource;

	response.addHeader("Date", this->dateHeader());
	response.addHeader("Server", "hsonseyu Server");

	int fd;
	if (checkPath(resource_path) == File) //있으면 append
	{
		if ((fd = open(resource_path.c_str(), O_WRONLY | O_APPEND | O_NONBLOCK, 0644)) == -1)
			this->makeErrorResponse(server, location, 500);
	}
	else if (checkPath(resource_path) == NotFound) //없으면 create
	{
		if ((fd = open(resource_path.c_str(), O_WRONLY | O_CREAT | O_NONBLOCK)) == -1)
			this->makeErrorResponse(server, location, 500);
	}
	else
		this->makeErrorResponse(server, location, 403);

	response.setResoureFD(fd);
	struct kevent event;

	EV_SET(&event, fd, EVFILT_WRITE, EV_ADD, NULL, NULL, NULL); //write event!
	return (response);
}

int		Server::deleteDirectory(std::string path)
{
	//디렉토리 오픈해서 하나하나 다 지우고 그 안에서도 파일이면 unlink, 디렉토리면 이 과정 반복
	DIR *dir;
	struct dirent *dir_read;

	if((dir = opendir(path.c_str())) == 0)
		return (1);
	
	while ((dir_read = readdir(dir)) != 0)
	{
		std::string name = std::string(file->d_name);
		if (name == "." || name == "..")
			continue ;
		if (dir_read->d_type == DT_DIR)
		{
			if (this->deleteDirectory(name) == 1)
				return (1);
		}
		else
			unlink(resource_path);
	}
	rmdir(path.c_str());
	return ;
}

Response Server::makeDELETEResponse(Location &location, std::string resource_path)
{
	//인자로 받은 resouece_path 는 이미 로케이션 내 root + 추가 경로까지 완성된 경로
	Response response;

	response.addHeader("Date", this->dateHeader());
	response.addHeader("Server", "hsonseyu Server");

	if (checkPath(resource_path) == Directory)
	{
		if (this->deleteDirectory(name) == 1)
			this->makeErrorResponsBody(500);
	}
	else if (checkPath(resource_path) == File)
		unlink(resource_path.c_str());

	response.addBody(this->makeDELETEResponseBody());

	return (response);
}

std::string	Server::statusMessage(size_t error_code) {
	std::map<size_t, std::string> status;

	status[100] = "Continue";
	status[101] = "Switching Protocols";
	status[102] = "Processing";
	status[200] = "OK";
	status[201] = "Created";
	status[202] = "Accepted";
	status[203] = "Non-authoritative Information";
	status[204] = "No Content";
	status[205] = "Reset Content";
	status[206] = "Partial Content";
	status[207] = "Multi-Status";
	status[208] = "Already Reported";
	status[226] = "IM Used";
	status[300] = "Multiple Choices";
	status[301] = "Moved Permanently";
	status[302] = "Found";
	status[303] = "See Other";
	status[304] = "Not Modified";
	status[305] = "Use Proxy";
	status[307] = "Temporary Redirect";
	status[308] = "Permanent Redirect";
	status[400] = "Bad Request";
	status[401] = "Unauthorized";
	status[402] = "Payment Required";
	status[403] = "Forbidden";
	status[404] = "Not found";
	status[405] = "Method Not Allowed";
	status[406] = "Not Acceptable";
	status[407] = "Proxy Authentication Required";
	status[408] = "Required Timeout";
	status[409] = "Conflict";
	status[410] = "Gone";
	status[411] = "Length Required";
	status[412] = "Precondition Failed";
	status[413] = "Request Entity Too Large";
	status[414] = "Request URI Too Long";
	status[415] = "Unsupported Media Type";
	status[416] = "Requested Range Not Satisfiable";
	status[417] = "Expectation Failed";
	status[418] = "IM_A_TEAPOT";
	status[500] = "Internal Server Error";
	status[501] = "Not Implemented";
	status[502] = "Bad Gateway";
	status[503] = "Service Unavailable";
	status[504] = "Gateway Timeout";
	status[505] = "HTTP Version Not Supported";
	status[506] = "Variant Also Negotiates";
	status[507] = "Insufficient Storage";
	status[508] = "Loop Detected";
	status[510] = "Not Extened";
	status[511] = "Network Authentication Required";
	status[599] = "Network Connect Timeout Error";
};
if (mimeType.count(extension) == 0)
		return "text/plain";
	else
		return mimeType[extension];
}

std::string	Server::contentTypeHeader(std::string extension) {
	std::map<std::string, std::string> mimeType;

	mimeType[".aac"] = "audio/aac";
	mimeType[".abw"] = "application/x-abiword";
	mimeType[".arc"] = "application/octet-stream";
	mimeType[".avi"] = "video/x-msvideo";
	mimeType[".azw"] = "application/vnd.amazon.ebook";
	mimeType[".bin"] = "application/octet-stream";
	mimeType[".bz"] = "application/x-bzip";
	mimeType[".bz2"] = "application/x-bzip2";
	mimeType[".csh"] = "application/x-csh";
	mimeType[".css"] = "text/css";
	mimeType[".csv"] = "text/csv";
	mimeType[".doc"] = "application/msword";
	mimeType[".epub"] = "application/epub+zip";
	mimeType[".Gif"] = "image/gif";
	mimeType[".htm"] = "text/html";
	mimeType[".html"] = "text/html";
	mimeType[".ico"] = "image/x-icon";
	mimeType[".ics"] = "text/calendar";
	mimeType[".jar"] = "Temporary Redirect";
	mimeType[".jpeg"] = "image/jpeg";
	mimeType[".jpg"] = "image/jpeg";
	mimeType[".js"] = "application/js";
	mimeType[".json"] = "application/json";
	mimeType[".mid"] = "audio/midi";
	mimeType[".midi"] = "audio/midi";
	mimeType[".mpeg"] = "video/mpeg";
	mimeType[".mpkg"] = "application/vnd.apple.installer+xml";
	mimeType[".odp"] = "application/vnd.oasis.opendocument.presentation";
	mimeType[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	mimeType[".odt"] = "application/vnd.oasis.opendocument.text";
	mimeType[".oga"] = "audio/ogg";
	mimeType[".ogv"] = "video/ogg";
	mimeType[".ogx"] = "application/ogg";
	mimeType[".pdf"] = "application/pdf";
	mimeType[".ppt"] = "application/vnd.ms-powerpoint";
	mimeType[".rar"] = "application/x-rar-compressed";
	mimeType[".rtf"] = "application/rtf";
	mimeType[".sh"] = "application/x-sh";
	mimeType[".svg"] = "image/svg+xml";
	mimeType[".swf"] = "application/x-shockwave-flash";
	mimeType[".tar"] = "application/x-tar";
	mimeType[".tif"] = "image/tiff";
	mimeType[".tiff"] = "image/tiff";
	mimeType[".ttf"] = "application/x-font-ttf";
	mimeType[".vsd"] = " application/vnd.visio";
	mimeType[".wav"] = "audio/x-wav";
	mimeType[".weba"] = "audio/webm";
	mimeType[".webm"] = "video/webm";
	mimeType[".webp"] = "image/webp";
	mimeType[".woff"] = "application/x-font-woff";
	mimeType[".xhtml"] = "application/xhtml+xml";
	mimeType[".xls"] = "application/vnd.ms-excel";
	mimeType[".xml"] = "application/xml";
	mimeType[".xul"] = "application/vnd.mozilla.xul+xml";
	mimeType[".zip"] = "application/zip";
	mimeType[".3gp"] = "video/3gpp audio/3gpp";
	mimeType[".3g2"] = "video/3gpp2 audio/3gpp2";
	mimeType[".7z"] = "application/x-7z-compressed";
	if (mimeType.count(extension) == 0)
		return "text/plain";
	else
		return mimeType[extension];
}






















// void Client::prepareResponse()
// {
// 	this->Response = portmanager.makeResponse(Request);
// 	if (!Response.hasResource())
// 		this->status = NEED_RESOURCE;
// 	else
// 		this->status = RESPONSE_COMPLETE;


// 	resouece 랑 response 랑 연결이되어있어야한다
// 	Server의 makeResponse 에서
// 	리소스의 경로를 찾고 파일이 있는지 확인을 하고 그러면 그걸 open 해서 fd 를 response 에 담아두기 ?
// 	그리고 서버매니저로가서 클라이언트 상태 확인해서 resource read / client write 를 켜기


// 	리소스가 리스폰스를 가져야하는 이유는 ?
// 	리소스를 읽을 때 리스폰스의 바디에 작성해야 하니까

// }

// Response PortManager::makeResponse(Request &req)
// {
// 	this->Servers.find(req.getHeaderValue("HOST"));
// 	Response response = server.makeResponse(req);

// 	return (response);
// }

// Response Server::makeResponse(Request &req)
// {
// 	Location location = location(req.getUri());

// 	//check
// 	Response response = response();
// 	//errorResponse
// 	//그냥 일반 200 response
// 	//cig response

// 	return (response);
// }