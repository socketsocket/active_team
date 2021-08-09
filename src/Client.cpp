#include "Client.hpp"

Client::Client(int socket_fd, PortManager *port_manager)
	: FDHandler(socket_fd),
	  reader(socket_fd),
	  processor(port_manager),
	  writer(NULL)
{}

Client::~Client()
{
	while (dialogues.empty() == false)
	{
		delete dialogues.front();
		dialogues.pop();
	}
}

void	Client::readRequest()
{
	//읽어서 RequestReader buffer 에 raw_request 를 담고
	//파싱 함수 실행

	char	buf[BUFFER_SIZE];
	int		len;

	len = read(this->getFD(), buf, BUFFER_SIZE - 1);
	if (len <= 0)
	{
		//error
	}
	buf[len] = 0;
	reader.getRawRequest() += buf;

	dialogues.push(reader.parseRequest());
}

