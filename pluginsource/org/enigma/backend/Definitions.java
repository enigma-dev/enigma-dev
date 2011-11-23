package org.enigma.backend;

import java.util.EnumMap;

import org.lateralgm.resources.InstantiableResource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.util.PropertyMap;

public class Definitions extends InstantiableResource<Definitions,Definitions.PDefinitions>
	{
	public enum PDefinitions
		{
		KIND,CODE
		}

	private static final EnumMap<PDefinitions,Object> DEFS = PropertyMap.makeDefaultMap(
			PDefinitions.class,0,new String());

	public Definitions()
		{
		this(null);
		}

	public Definitions(ResourceReference<Definitions> ref)
		{
		super(ref);
		}

	@Override
	public Definitions makeInstance(ResourceReference<Definitions> ref)
		{
		return new Definitions(ref);
		}

	@Override
	protected PropertyMap<PDefinitions> makePropertyMap()
		{
		return new PropertyMap<PDefinitions>(PDefinitions.class,this,DEFS);
		}
	}
