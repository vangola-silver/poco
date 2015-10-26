//
// Client.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Client
//
// Implementation of the Client class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/Client.h"
#include "Poco/Redis/Exception.h"

namespace Poco {
namespace Redis {


Client::Client() : _address(), _socket()
{
}


Client::Client(const std::string& hostAndPort) : _address(hostAndPort), _socket()
{
	connect();
}


Client::Client(const std::string& host, int port) : _address(host, port), _socket()
{
	connect();
}


Client::Client(const Net::SocketAddress& addrs) : _address(addrs), _socket()
{
	connect();
}


Client::~Client()
{
}


void Client::connect()
{
	_socket.connect(_address);
}


void Client::connect(const std::string& hostAndPort)
{
	_address = Net::SocketAddress(hostAndPort);
	connect();
}


void Client::connect(const std::string& host, int port)
{
	_address = Net::SocketAddress(host, port);
	connect();
}


void Client::connect(const Net::SocketAddress& addrs)
{
	_address = addrs;
	connect();
}


void Client::disconnect()
{
	_socket.close();
}

void Client::writeCommand(const Array& command)
{
	std::string commandStr = command.toString();
	_socket.write(commandStr.c_str(), commandStr.length());
}

RedisType::Ptr Client::readReply()
{
	RedisType::Ptr result = RedisType::createRedisType(_socket.get());
	if ( result.isNull() )
	{
		throw RedisException("Invalid Redis type returned");
	}

	result->read(_socket);

	return result;
}

RedisType::Ptr Client::sendCommand(const Array& command)
{
	writeCommand(command);
	return readReply();
}

void Client::sendCommands(const std::vector<Array>& commands, Array& results)
{
	for(std::vector<Array>::const_iterator it = commands.begin(); it != commands.end(); ++it)
	{
		writeCommand(*it);
	}

	for(int i = 0; i < commands.size(); ++i)
	{
		results.add(readReply());
	}
}


} } // Poco::Redis
