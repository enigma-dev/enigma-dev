/**
 *  @file json.cpp
 *  @section License
 *
 *      Copyright (C) 2013 Ssss
 *      Copyright (C) 2023 Saif Kandil
 *
 *      This file is a part of the ENIGMA Development Environment.
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "json.h"
#include "../DataStructures/include.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cstdio>
#include <iostream>
#include <sstream>

namespace enigma {
	// int RecursiveDSList(Json::Value root); 

	int RecursiveDSMap(Json::Value root)
	{
		unsigned jsonObject= enigma_user::ds_map_create();

		Json::Value::Members m=root.getMemberNames();
		for (Json::Value::Members::iterator it = m.begin() ; it != m.end(); ++it)
		{
			if (root[*it].isNull())
			{
				enigma_user::ds_map_add(jsonObject,*it,0);
			}
			else if (root[*it].isBool())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asInt());
			}
			else if (root[*it].isInt())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asInt());
			}
			else if (root[*it].isUInt())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asUInt());
			}
			else if (root[*it].isIntegral())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asInt());
			}
			else if (root[*it].isDouble())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asDouble());
			}
			else if (root[*it].isNumeric())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asDouble());
			}
			else if (root[*it].isString())
			{
				enigma_user::ds_map_add(jsonObject,*it,root[*it].asString());
			}
			else if (root[*it].isArray())
			{
				int dsmap=RecursiveDSList(root[*it]);
				if (dsmap==-1)
				{
					return -1;
				}
				enigma_user::ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isObject())
			{
				int dsmap=RecursiveDSMap(root[*it]);
				if (dsmap==-1)
				{
					return -1;
				}
				enigma_user::ds_map_add(jsonObject,*it,dsmap);
			}
			else
			{
				return -1;
			}
		}
		return jsonObject;
	}
	
	int RecursiveDSList(Json::Value root)
	{
		unsigned jsonArray=enigma_user::ds_list_create();

		for (unsigned i=0; i<root.size(); ++i)
		{
			if (root[i].isNull())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), 0));
			}
			else if (root[i].isBool())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asInt()));
			}
			else if (root[i].isInt())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asInt()));
			}
			else if (root[i].isUInt())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asUInt()));
			}
			else if (root[i].isIntegral())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asInt()));
			}
			else if (root[i].isDouble())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asDouble()));
			}
			else if (root[i].isNumeric())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asDouble()));
			}
			else if (root[i].isString())
			{
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), root[i].asString()));
			}
			else if (root[i].isArray())
			{
				int dsmap=RecursiveDSList(root[i]);
				if (dsmap==-1)
				{
					return -1;
				}
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isObject())
			{
				int dsmap=RecursiveDSMap(root[i]);
				if (dsmap==-1)
				{
					return -1;
				}
				enigma_user::ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else
			{
				return -1;
			}
		}
		return jsonArray;
	}

	// string InverseRecursiveDSList(int ds_list);

	std::string InverseRecursiveDSMap(int ds_map)
	{
		std::stringstream encoding_accumulator;

		encoding_accumulator << '{';

		variant key{enigma_user::ds_map_find_first(ds_map)};

		// DEBUG_MESSAGE("step -1", MESSAGE_TYPE::M_INFO);

		for (unsigned i = 0; i < enigma_user::ds_map_size(ds_map); i++) {
			// DEBUG_MESSAGE("step "+std::to_string(i), MESSAGE_TYPE::M_INFO);
			encoding_accumulator << '\"' << std::string(key) << '\"' << ':';

			variant value {enigma_user::ds_map_find_value(ds_map, key)};

			if (enigma_user::is_string(value))
			{
				encoding_accumulator << '\"' << enigma_user::toString(value) << '\"';
			}
			else if (enigma_user::is_real(value))
			{
				if (enigma_user::ds_map_exists(value)) {
					std::string temp = enigma::InverseRecursiveDSMap(value);
					encoding_accumulator << temp;
				}
				else if (enigma_user::ds_list_exists(value)){
					std::string temp = enigma::InverseRecursiveDSList(value);
					encoding_accumulator << temp;
				}
				else {
					encoding_accumulator << enigma_user::toString(value);
				}
			}
			else {
				return std::string("{}");
			}

			key = enigma_user::ds_map_find_next(ds_map, key);
			
			// Add comma if not last element
			if (i != enigma_user::ds_map_size(ds_map) - 1) encoding_accumulator << ',';
		}

		encoding_accumulator << '}';

		return encoding_accumulator.str();
	}

	std::string InverseRecursiveDSList(int ds_list)
	{
		std::stringstream encoding_accumulator;

		encoding_accumulator << '[';

		for (unsigned i = 0; i < enigma_user::ds_list_size(ds_list); i++) {
			variant value {enigma_user::ds_list_find_value(ds_list, i)};

			if (enigma_user::is_string(value))
			{
				encoding_accumulator << '\"' << enigma_user::toString(value) << '\"';
			}
			else if (enigma_user::is_real(value))
			{
				if (enigma_user::ds_map_exists(value)) {
					std::string temp = enigma::InverseRecursiveDSMap(value);
					encoding_accumulator << temp;
				}
				else if (enigma_user::ds_list_exists(value)){
					std::string temp = enigma::InverseRecursiveDSList(value);
					encoding_accumulator << temp;
				}
				else {
					encoding_accumulator << enigma_user::toString(value);
				}
			}
			else {
				return std::string("[]");
			}
			
			// Add comma if not last element
			if (i != enigma_user::ds_list_size(ds_list) - 1) encoding_accumulator << ',';
		}

		encoding_accumulator << ']';
    
		return encoding_accumulator.str();
	}
}

namespace enigma_user
{
	variant json_decode(std::string data)
	{
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( std::string(data), root );
		if ( !parsingSuccessful )
		{
			DEBUG_MESSAGE("Failed to parse configuration" + reader.getFormattedErrorMessages(), MESSAGE_TYPE::M_ERROR);
			return -1;
		}

		if (root.isObject())
		{
			return enigma::RecursiveDSMap(root);
		}
		else if (root.isArray())
		{
			return enigma::RecursiveDSList(root);
		}
		else
		{
			DEBUG_MESSAGE("Invalid JSON. The root is not as array of objects or an object.", MESSAGE_TYPE::M_ERROR);
			return -1;
		}
	}

	/*
		We are missing a DS accessors here. json_encode() can't be implemented before add DS accessors to
		DataStructures extension.
	*/
	std::string json_encode(variant ds) {
		if (enigma_user::ds_map_exists(ds)) {
			DEBUG_MESSAGE("DS is an object", MESSAGE_TYPE::M_INFO);
			return enigma::InverseRecursiveDSMap(ds);
		}
		else if (enigma_user::ds_list_exists(ds)) {
			DEBUG_MESSAGE("DS is an array", MESSAGE_TYPE::M_INFO);
			return enigma::InverseRecursiveDSList(ds);
		}
		else {
			DEBUG_MESSAGE("DS map does not exist", MESSAGE_TYPE::M_ERROR);
			return std::string("{}");
		}
	}
}
