/*
 * Copyright (C) 2007, 2008 Quadduc <quadduc@gmail.com>
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of ENIGMA.
 * 
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

package org.enigma.messages;

import java.text.MessageFormat;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

public final class Messages
	{
	private static final String BUNDLE_NAME = "org.enigma.messages.messages"; //$NON-NLS-1$

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle(BUNDLE_NAME);

	private Messages()
		{
		}

	public static String getString(String key)
		{
		try
			{
			return RESOURCE_BUNDLE.getString(key);
			}
		catch (MissingResourceException e)
			{
			return '!' + key + '!';
			}
		}

	public static String format(String key, Object...arguments)
		{
		try
			{
			String p = RESOURCE_BUNDLE.getString(key);
			return MessageFormat.format(p,arguments);
			}
		catch (MissingResourceException e)
			{
			return '!' + key + '!';
			}
		}
	}
