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

#include "Widget_Systems/widgets_mandatory.h"
#include "json.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include "../DataStructures/include.h"
#include "libjson.h"

using std::string;

namespace enigma_user
{
	int RecursiveDSList(Json::Value value);

	int RecursiveDSMap(Json::Value root)
	{
		int jsonObject=ds_map_create();

		Json::Value::Members m=root.getMemberNames();
		for (Json::Value::Members::iterator it = m.begin() ; it != m.end(); ++it)
		{
			if (root[*it].isNull())
			{
				ds_map_add(jsonObject,*it,0);
			}
			else if (root[*it].isBool())
			{
				ds_map_add(jsonObject,*it,root[*it].asInt());
			}
			else if (root[*it].isInt())
			{
				ds_map_add(jsonObject,*it,root[*it].asInt());
			}
			else if (root[*it].isUInt())
			{
				ds_map_add(jsonObject,*it,root[*it].asUInt());
			}
			else if (root[*it].isIntegral())
			{
				ds_map_add(jsonObject,*it,root[*it].asInt());
			}
			else if (root[*it].isDouble())
			{
				ds_map_add(jsonObject,*it,root[*it].asDouble());
			}
			else if (root[*it].isNumeric())
			{
				ds_map_add(jsonObject,*it,root[*it].asDouble());
			}
			else if (root[*it].isString())
			{
				ds_map_add(jsonObject,*it,root[*it].asString());
			}
			else if (root[*it].isArray())
			{
				int dsmap=RecursiveDSList(root[*it]);
				if (dsmap==-1)
				{
					return -1;
				}
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isObject())
			{
				int dsmap=RecursiveDSMap(root[*it]);
				if (dsmap==-1)
				{
					return -1;
				}
				ds_map_add(jsonObject,*it,dsmap);
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
		int jsonArray=ds_list_create();

		for (int i=0; i<root.size(); ++i)
		{
			if (root[i].isNull())
			{
				ds_list_add(jsonArray,(enigma::varargs(), 0));
			}
			else if (root[i].isBool())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asInt()));
			}
			else if (root[i].isInt())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asInt()));
			}
			else if (root[i].isUInt())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asUInt()));
			}
			else if (root[i].isIntegral())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asInt()));
			}
			else if (root[i].isDouble())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asDouble()));
			}
			else if (root[i].isNumeric())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asDouble()));
			}
			else if (root[i].isString())
			{
				ds_list_add(jsonArray,(enigma::varargs(), root[i].asString()));
			}
			else if (root[i].isArray())
			{
				int dsmap=RecursiveDSList(root[i]);
				if (dsmap==-1)
				{
					return -1;
				}
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isObject())
			{
				int dsmap=RecursiveDSMap(root[i]);
				if (dsmap==-1)
				{
					return -1;
				}
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else
			{
				return -1;
			}
		}
		return jsonArray;
	}
	
	variant json_decode(string data)
	{
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( string(data), root );
		if ( !parsingSuccessful )
		{
			DEBUG_MESSAGE("Failed to parse configuration" + reader.getFormattedErrorMessages(), MESSAGE_TYPE::M_ERROR);
			return -1;
		}

		if (root.isObject())
		{
			return RecursiveDSMap(root);
		}
		else if (root.isArray())
		{
			return RecursiveDSList(root);
		}
		else
		{
			DEBUG_MESSAGE("Invalid JSON. The root is not as array of objects or an object.", MESSAGE_TYPE::M_ERROR);
			return -1;
		}
	}

	string json_encode(variant ds_map) {
		if (!enigma_user::ds_map_exists(ds_map)) {
				DEBUG_MESSAGE("DS map does not exist", MESSAGE_TYPE::M_ERROR);
				return string("{}");
		}

		std::stringstream encoding_accumulator;

		encoding_accumulator << '{';

		variant key{enigma_user::ds_map_find_first(ds_map)};

		for (int i = 0; i < enigma_user::ds_map_size(ds_map); i++) {
			encoding_accumulator << '\"' << string(key) << '\"' << ':';

			variant value {enigma_user::ds_map_find_value(ds_map, key)};

			if (enigma_user::is_string(value))
				encoding_accumulator << '\"' << enigma_user::toString(value) << '\"';
			else 
				encoding_accumulator << enigma_user::toString(value);

			key = enigma_user::ds_map_find_next(ds_map, key);
			
			// Add comma if not last element
			if (i != enigma_user::ds_map_size(ds_map) - 1) encoding_accumulator << ',';
		}

		encoding_accumulator << '}';

		return encoding_accumulator.str();
	}
}
