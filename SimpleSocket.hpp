/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melperri <melperri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:14:12 by melperri          #+#    #+#             */
/*   Updated: 2022/04/04 18:14:12 by melperri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>

class SimpleSocket {
	public:

		SimpleSocket();
		SimpleSocket(const SimpleSocket &simplesocket);
		~SimpleSocket();

		SimpleSocket &operator=(const SimpleSocket &simplesocket);

	private:
};

#endif
