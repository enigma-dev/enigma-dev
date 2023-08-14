/**
 *  @file json.cpp
 *  @section License
 *
 *      Copyright (C) 2013 Ssss
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
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",0);
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isBool())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asInt());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isInt())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asInt());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isUInt())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asUInt());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isIntegral())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asInt());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isDouble())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asDouble());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isNumeric())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asDouble());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isString())
			{
				//ds_map with key "default" and value
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[*it].asString());
				ds_map_add(jsonObject,*it,dsmap);
			}
			else if (root[*it].isArray())
			{
				//ds_map with key "default" and ds_list of the objects or values
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
		int jsonObject=ds_map_create();
		int jsonArray=ds_list_create();
		ds_map_add(jsonObject,"default",jsonArray);

		for (int i=0; i<root.size(); ++i)
		{
			if (root[i].isNull())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",0);
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isBool())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asInt());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isInt())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asInt());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isUInt())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asUInt());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isIntegral())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asInt());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isDouble())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asDouble());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isNumeric())
			{
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asDouble());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isString())
			{
				//ds_map with key "default" and value
				int dsmap=ds_map_create();
				ds_map_add(dsmap,"default",root[i].asString());
				ds_list_add(jsonArray,(enigma::varargs(), dsmap));
			}
			else if (root[i].isArray())
			{
				//ds_map with key "default" and ds_list of the objects or values
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
		return jsonObject;
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
		return RecursiveDSMap(root);
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

			// DEBUG_MESSAGE(value, MESSAGE_TYPE::M_INFO);
		}

		encoding_accumulator << '}';

		// DEBUG_MESSAGE(encoding_accumulator, MESSAGE_TYPE::M_INFO);

		return encoding_accumulator.str();
	}
}
