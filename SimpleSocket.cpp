/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melperri <melperri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:14:47 by melperri          #+#    #+#             */
/*   Updated: 2022/04/04 18:14:47 by melperri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "SimpleSocket.hpp"

SimpleSocket::SimpleSocket() {
	std::cout << "Default constructor called" << std::endl;
	return ;
}

SimpleSocket::SimpleSocket(const SimpleSocket &simplesocket) {
	std::cout << "Copy constructor called" << std::endl;
	*this = simplesocket;
	return ;
}

SimpleSocket::~SimpleSocket() {
	std::cout << "Destructor called" << std::endl;
	return ;
}

SimpleSocket &SimpleSocket::operator=(const SimpleSocket &simplesocket) {
	std::cout << "Copy assignement operator called" << std::endl;
	return *this;
}
