#include <unistd.h>

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>

#define CHUNK_LEN	8162UL
#define MAX_OFFSET	8162L

static void
	printHeader(void)
{
	std::cout << "Content-Type: text/html" << std::endl;
	std::cout << std::endl;
}

static void
	printHTML(const std::string &html)
{
	std::cout << html;
	return ;
}

static std::string
	makeHTML(
		std::ifstream &in,
		double total_duration,
		double top_duration,
		double bot_duration,
		std::string top_string,
		std::string bot_string,
		std::string top_color,
		std::string bot_color )
{
	std::stringstream	html;

	html << "<!DOCTYPE html>" << std::endl;
	html << "<html lang=\"en\">" << std::endl;
	html << "<head>" << std::endl;
	html << "	<style>" << std::endl;
	html << "		/* reset */" << std::endl;
	html << "		*," << std::endl;
	html << "		*::before," << std::endl;
	html << "		*::after {" << std::endl;
	html << "		margin: 0;" << std::endl;
	html << "		padding: 0;" << std::endl;
	html << "		box-sizing: border-box;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		body {" << std::endl;
	html << "		display: flex;" << std::endl;
	html << "		flex-direction: column;" << std::endl;
	html << "		justify-content: space-around;" << std::endl;
	html << "		align-items: center;" << std::endl;
	html << "" << std::endl;
	html << "		min-height: 100vh;" << std::endl;
	html << "		padding: 20px;" << std::endl;
	html << "" << std::endl;
	html << "		font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "" << std::endl;
	html << "		/* other */" << std::endl;
	html << "		.info {" << std::endl;
	html << "		margin: 20px 0;" << std::endl;
	html << "		text-align: center;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		p {" << std::endl;
	html << "		color: #2e2e2e;" << std::endl;
	html << "		margin-bottom: 20px;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "" << std::endl;
	html << "		/* block-$ */" << std::endl;
	html << "		.block-effect {" << std::endl;
	html << "		font-size: calc(8px + 6vw);" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		.block-reveal {" << std::endl;
	html << "		--t: calc(var(--td) + var(--d));" << std::endl;
	html << "" << std::endl;
	html << "		color: transparent;" << std::endl;
	html << "		padding: 4px;" << std::endl;
	html << "" << std::endl;
	html << "		position: relative;" << std::endl;
	html << "		overflow: hidden;" << std::endl;
	html << "" << std::endl;
	html << "		animation: revealBlock 0s var(--t) forwards;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		.block-reveal::after {" << std::endl;
	html << "		content: '';" << std::endl;
	html << "" << std::endl;
	html << "		width: 0%;" << std::endl;
	html << "		height: 100%;" << std::endl;
	html << "		padding-bottom: 4px;" << std::endl;
	html << "" << std::endl;
	html << "		position: absolute;" << std::endl;
	html << "		top: 0;" << std::endl;
	html << "		left: 0;" << std::endl;
	html << "" << std::endl;
	html << "		background: var(--bc);" << std::endl;
	html << "		animation: revealingIn var(--td) var(--d) forwards, revealingOut var(--td) var(--t) forwards;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "" << std::endl;
	html << "		/* animations */" << std::endl;
	html << "		@keyframes revealBlock {" << std::endl;
	html << "		100% {" << std::endl;
	html << "			color: #0f0f0f;" << std::endl;
	html << "		}" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		@keyframes revealingIn {" << std::endl;
	html << "" << std::endl;
	html << "		0% {" << std::endl;
	html << "			width: 0;" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		100% {" << std::endl;
	html << "			width: 100%;" << std::endl;
	html << "		}" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		@keyframes revealingOut {" << std::endl;
	html << "" << std::endl;
	html << "		0% {" << std::endl;
	html << "			transform: translateX(0);" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		100% {" << std::endl;
	html << "			transform: translateX(100%);" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		}" << std::endl;
	html << "" << std::endl;
	html << "		.abs-site-link {" << std::endl;
	html << "		position: fixed;" << std::endl;
	html << "		bottom: 20px;" << std::endl;
	html << "		left: 20px;" << std::endl;
	html << "		color: hsla(0, 0%, 0%, .6);" << std::endl;
	html << "		font-size: 16px;" << std::endl;
	html << "		}" << std::endl;
	html << "	</style>" << std::endl;
	html << "" << std::endl;
	html << "	<meta charset=\"UTF-8\">" << std::endl;
	html << "	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << std::endl;
	html << "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << std::endl;
	html << "	<title>Document</title>" << std::endl;
	html << "</head>" << std::endl;
	html << "<body>" << std::endl;
	html << "	<h1 class=\"block-effect\" style=\"--td: " << total_duration << "s\">" << std::endl;
	html << "		<div class=\"block-reveal\" style=\"--bc: " << top_color << "; --d: " << top_duration << "s\">" << top_string << "</div>" << std::endl;
	html << "		<div class=\"block-reveal\" style=\"--bc: " << bot_color << "; --d: " << bot_duration << "s\">" << bot_string << "</div>" << std::endl;
	html << "	</h1>" << std::endl;
	html << "" << std::endl;
	html << "	<div class=\"info\">" << std::endl;

	std::string	str;
	while (in >> str)
		html << "		<p>" << str << "</p>" << std::endl;

	html << "	</div>" << std::endl;
	html << "" << std::endl;
	html << "	<!--<a href=\"https://abubakersaeed.netlify.app/designs/d12-block-revealing-effect\" class=\"abs-site-link\" rel=\"nofollow noreferrer\" target=\"_blank\">abs/designs/d12-block-revealing-effect</a> -->" << std::endl;
	html << "</body>" << std::endl;
	html << "</html>" << std::endl;

	return (html.str());
}

static void
	parseQuery(
		std::stringstream	&ss,
		double				&total_duration,
		double				&top_duration,
		double				&bot_duration,
		std::string			&top_string,
		std::string			&bot_string,
		std::string			&top_color,
		std::string			&bot_color )
{
	std::string	query;

	while (getline(ss, query, '&'))
	{
		std::string::size_type	asg_idx;
		std::string				key;
		std::string				val;

		if ((asg_idx = query.find('=')) == query.npos)
			continue ;
		else if (asg_idx + 1 == query.length())
			continue ;
		key = query.substr(0, asg_idx);
		val = query.substr(asg_idx + 1);
		if (key == "total_duration")
			total_duration = strtod(val.c_str(), NULL);
		else if (key == "top_duration")
			top_duration = strtod(val.c_str(), NULL);
		else if (key == "bot_duration")
			bot_duration = strtod(val.c_str(), NULL);
		else if (key == "top_string")
			top_string = val;
		else if (key == "bot_string")
			bot_string = val;
		else if (key == "top_color")
			top_color = val;
		else if (key == "bot_color")
			bot_color = val;
	}
}

static std::string
	makeErrorHTML(void)
{
	std::stringstream	html;

	html << "<!DOCTYPE html>" << std::endl;
	html << "<html lang=\"en\">" << std::endl;
	html << "<head>" << std::endl;
	html << "	<meta charset=\"UTF-8\">" << std::endl;
	html << "	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << std::endl;
	html << "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << std::endl;
	html << "	<title>Error</title>" << std::endl;
	html << "</head>" << std::endl;
	html << "<body>" << std::endl;
	html << "	Something wrong ..." << std::endl;
	html << "</body>" << std::endl;
	html << "</html>" << std::endl;
	return (html.str());
}

static void
	getQueryStream(std::stringstream &query_stream)
{
	char				*method;

	method = getenv("REQUEST_METHOD");
	if (method)
	{
		if (strcmp(method, "GET") == 0)
		{
			if (getenv("QUERY_STRING"))
				query_stream.str(getenv("QUERY_STRING"));
			else
				query_stream.str("");
		}
		else if (strcmp(method, "POST") == 0)
		{
			ssize_t	read_size = strtol(getenv("CONTENT_LENGTH"), NULL, 10);
			ssize_t	read_bytes;
			char	buffer[read_size + 1];

			while ((read_bytes = read(0, buffer, std::min(read_size, MAX_OFFSET))) > 0)
			{
				read_size -= read_bytes;
				buffer[read_bytes] = '\0';
				query_stream << buffer;
			}
		}
		else
			throw makeErrorHTML();
	}
	else
		throw makeErrorHTML();
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (-1);

	std::ofstream	log("./cgi/reveal_cgi.log", std::ios::out | std::ios::app);
	if (log.is_open() == false)
		return (-1);

	printHeader();

	std::stringstream	query_stream;

	double		total_duration	= 1.2;
	double		top_duration	= 0.1;
	double		bot_duration	= 0.5;
	std::string	top_string		= "CGI Tester";
	std::string	bot_string		= "hson seyu webserv";
	std::string	top_color		= "#4040bf";
	std::string	bot_color		= "#bf4060";

	try
	{
		getQueryStream(query_stream);
		log << "Query string = " << query_stream.str() << std::endl;
	}
	catch (std::string &e)
	{
		printHTML(e);
		log << "ERROR: Cannot get query stream" << std::endl;
		log.close();
		return (1);
	}

	std::ifstream	in(argv[1]);
	if (in.is_open() == false)
	{
		printHTML(makeErrorHTML());
		log << "ERROR: Cannot open file: " << argv[1] << std::endl;
		log.close();
		return (1);
	}

	log << "Target file : " << argv[1] << std::endl;
	log << total_duration << std::endl;
	log << top_duration << std::endl;
	log << bot_duration << std::endl;
	log << top_string << std::endl;
	log << bot_string << std::endl;
	log << top_color << std::endl;
	log << bot_color << std::endl;
	log << std::endl;

	parseQuery(query_stream, total_duration, top_duration, bot_duration, top_string, bot_string, top_color, bot_color);
	printHTML(makeHTML(in, total_duration, top_duration, bot_duration, top_string, bot_string, top_color, bot_color));
	in.close();
	log.close();
	return (0);
}
