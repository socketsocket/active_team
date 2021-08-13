#include "Server.hpp"
#include "EventHandlerInstance.hpp"

Server::Server()
{
}

Server::~Server()
{
}

//config parsing
void
	Server::addLocation(std::string path, Location *loc)
{
	this->locations.insert(std::make_pair(path, loc));
}

void
	Server::addErrorPage(int error_code, std::string page_path)
{
	this->error_page.insert(std::make_pair(error_code, page_path));
}

void
	Server::setBodyLimit(int limit)
{
	this->body_limit = limit;
}

void
	Server::setReturnInfo(int code, std::string uri)
{
	this->return_info = std::make_pair(code, uri);
}

void
	Server::setAutoindex(std::string on_off_string)
{
	if (on_off_string == "on")
		auto_index = true;
	else if (on_off_string == "off")
		auto_index = false;
	else
		throw BadDirective(on_off_string);
}

//make Response
std::string 
	Server::makeHTMLPage(std::string content)
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

std::string
	Server::generateErrorPage(size_t error_code)
{
	std::string html;

	html += "<html>\r\n";

	html += "<head>\r\n";
	html += "<title>" + std::to_string(error_code) + " " + this->statusMessage(error_code) + "</head>\r\n";
	html += "</head>\r\n";

	html += "<body>\r\n";
	html += "<center>\r\n";
	html += "<h1>" + std::to_string(error_code) + " " + this->statusMessage(error_code) + "</h1";
	html += "</center>\r\n";;
	html += "<hr>\r\n";
	html += "<center> hsonseyu/1.1</center>";
	html += "</body>\r\n";

	html += "</html>";
	
	return (html);
}

void Server::makeErrorResponse(Dialogue *dial, Location &location, size_t error_code)
{
	Response &response = dial->res;
	response.makeStartLine("HTTP/1.1", error_code, this->statusMessage(error_code));
	response.addHeader(std::string("Date"), this->dateHeader());
	response.addHeader(std::string("Server"), "hsonseyu Server");
	response.addHeader(std::string("Content-Type"), this->contentTypeHeader(".html"));

	int fd = 0;
	// struct stat buf;

	if (location.getErrorPages().count(error_code))
		fd = open(location.getErrorPages()[error_code].c_str(), O_RDONLY);
	else if (this->getErrorPages().count(error_code))
		fd = open(this->getErrorPages()[error_code].c_str(), O_RDONLY);
	else
		response.addBody(this->generateErrorPage(error_code));

	if (fd == -1)
		response.addBody(this->generateErrorPage(error_code));
	else
	{
		Resource	resource(fd, dial);
		EventHandlerInstance::getInstance().enableReadEvent(resource.getFD());
		return ;
	}

	dial->status = Dialogue::READY_TO_RESPONSE;
}

void	Server::makeReturnResponse(Dialogue *dial, Location &location, size_t return_code)
{
	Response &response = dial->res;
	response.makeStartLine("HTTP/1.1", return_code, this->statusMessage(return_code));
	response.addHeader(std::string("Date"), this->dateHeader());
	response.addHeader(std::string("Server"), "hsonseyu Server");
	response.addHeader(std::string("Location"), location.getReturnInfo().second);
}


std::string	Server::makeAutoIndexPage(std::string path, std::string uri, Location &location)
{
	(void)location;
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

	if((dir = opendir(path.c_str())) == 0)
		return (this->generateErrorPage(500));
	
	while ((dir_read = readdir(dir)) != 0)
	{
		std::string name = std::string(dir_read->d_name);
		if (dir_read->d_type == DT_DIR)
			name += '/';
		body += "<a href=\"" + name + "\">" + name + "</a><br>\n";

		//dd-mm-yyyy hh:mm
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

	return (body);
}


void Server::makeGETResponse(Dialogue *dial, Location &location, std::string path)
{
	Response &response = dial->res;
	response.addHeader("Date", this->dateHeader());
	response.addHeader("Server", "hsonseyu Server");

	if (checkPath(path) == Directory)
	{
		//Check Index Files
		if (path[path.length() - 1] != '/')
			path += '/';
		struct stat buf;
		bool found = false;
		for (std::vector<std::string>::iterator iter = location.getIndex().begin(); iter != location.getIndex().end(); iter++)
		{
			if (stat((path + *iter).c_str(), &buf) == 0)
			{
				found = true;
				path = path + *iter;
				break ;
			}
		}
		if (found == false && location.isAutoIndex() == true)
		{
			response.addHeader("Content-Type", this->contentTypeHeader(".html"));
			response.addBody(this->makeAutoIndexPage(path, path, location));
			response.addHeader("Content-Length", std::to_string(response.getBody().length()));
			//200 ok
		}
		if (checkPath(path) == NotFound || checkPath(path) == Directory)
			return this->makeErrorResponse(dial, location, 404);
	}

	//경로가 특정 파일로 지정됨! 오픈!
	int	fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
		return this->makeErrorResponse(dial, location, 404);

	response.makeStartLine("HTTP/1.1", 200, this->statusMessage(200));

	Resource resource(fd, dial);
	EventHandlerInstance::getInstance().enableReadEvent(resource.getFD());
}


void Server::makePOSTResponse(Dialogue *dial, Location &location, std::string resource_path)
{
	//POST 는 대부분 cgi 처리를 원함. cgi 가 아닌 서버에서의 POST 의 경우 파일 생성
	Response &response = dial->res;

	response.addHeader("Date", this->dateHeader());
	response.addHeader("Server", "hsonseyu Server");

	int fd;
	if (checkPath(resource_path) == File) //있으면 append
	{
		if ((fd = open(resource_path.c_str(), O_WRONLY | O_APPEND | O_NONBLOCK, 0644)) == -1)
			return this->makeErrorResponse(dial, location, 500);
	}
	else if (checkPath(resource_path) == NotFound) //없으면 create
	{
		if ((fd = open(resource_path.c_str(), O_WRONLY | O_CREAT | O_NONBLOCK)) == -1)
			return this->makeErrorResponse(dial, location, 500);
	}
	else
		return this->makeErrorResponse(dial, location, 403);

	response.makeStartLine("HTTP/1.1", 201, this->statusMessage(201));

	Resource resource(fd, dial);
	EventHandlerInstance::getInstance().enableReadEvent(resource.getFD());
}

void Server::makeDELETEResponse(Dialogue *dial, Location &location, std::string resource_path)
{
	(void)location;
	//인자로 받은 resouece_path 는 이미 로케이션 내 root + 추가 경로까지 완성된 경로
	Response &response = dial->res;

	response.addHeader("Date", this->dateHeader());
	response.addHeader("Server", "hsonseyu Server");

	if (checkPath(resource_path) == Directory)
	{
		if (this->deleteDirectory(resource_path) == 1)
			response.addBody(this->generateErrorPage(500));
	}
	else if (checkPath(resource_path) == File)
		unlink(resource_path.c_str());

	response.makeStartLine("HTTP/1.1", 200, this->statusMessage(200));
	response.addBody(this->makeHTMLPage("File deleted."));
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
		else
			throw	BadRequest();
	}
}

//디렉토리 오픈해서 다 지우는데 그 안에서도 파일이면 unlink, 디렉토리면 이 과정 반복
int		Server::deleteDirectory(std::string path)
{
	DIR *dir;
	struct dirent *dir_read;

	if((dir = opendir(path.c_str())) == 0)
		return (1);
	
	while ((dir_read = readdir(dir)) != 0)
	{
		std::string name = std::string(dir_read->d_name);
		if (name == "." || name == "..")
			continue ;
		if (dir_read->d_type == DT_DIR)
		{
			if (this->deleteDirectory(name) == 1)
				return (1);
		}
		else
			unlink(path.c_str());
	}
	rmdir(path.c_str());
	return (0);
}

std::string	Server::statusMessage(size_t code) {
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
	if (status.count(code) == 0)
		return 0;
	else
		return status[code];
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

std::string	
	Server::dateHeader(void)
{
	time_t curr_time = time(NULL);
	struct tm* time_info = localtime(&curr_time);
	char buffer[4096];

	strftime(buffer, 4096, "%a, %d %b %Y %H:%M:%S GMT", time_info);
	return (std::string(buffer));
}

std::string
	Server::lastModifiedHeader(void) 
{
	struct stat	sb;
	struct tm*	timeinfo = localtime(&sb.st_mtime);
	char buffer[4096];

	strftime(buffer, 4096, "%a, %d, %b %Y %X GMT", timeinfo);

	return (std::string(buffer));
}

std::string
	Server::connectionHeader(Request &req)
{
	std::map<std::string, std::string>::iterator iter = req.getHeaders().find("Connection-Encoding");
	if (iter != req.getHeaders().end())
		return (iter->second);
	else
		return ("keep-alive");
}

Location*
	Server::getLocation(std::string uri)
{
	std::map<std::string, Location*>::iterator iter = locations.find(uri);
	if (iter == locations.end())
		return (0);
	return (iter->second);
}

unsigned int
	Server::getBodyLimit()
{
	return (body_limit);
}

size_t
	Server::getReturnCode()
{
	return (return_info.first);
}

std::map<int, std::string>
	Server::getErrorPages()
{
	return (error_page);
}


