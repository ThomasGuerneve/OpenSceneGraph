/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#include <osg/GL>
#include <osg/PolygonOffset>
#include <osg/Notify>

using namespace osg;

static float s_FactorMultipler = 1.0f;
static float s_UnitMultipler = 1.0f;
static bool s_MultiplerSet = false;

void PolygonOffset::setFactorMultiplier(float multiplier)
{
    s_MultiplerSet = true;
    s_FactorMultipler = multiplier;
}

float PolygonOffset::getFactorMultiplier()
{
    return s_FactorMultipler;
}

void PolygonOffset::setUnitMultiplier(float multiplier)
{
    s_MultiplerSet = true;
    s_UnitMultipler = multiplier;
}

float PolygonOffset::getUnitMultiplier()
{
    return s_UnitMultipler;
}

bool PolygonOffset::areUnitAndMultipliersSet()
{
    return s_MultiplerSet;
}


void PolygonOffset::setFactorAndUnitMultipliersUsingBestGuessForDriver()
{
    s_MultiplerSet = true;
    // osg::notify(osg::NOTICE)<<"PolygonOffset::setFactorAndUnitMultipliersUsingBestGuessForDriver()"<<std::endl;

    const GLubyte* renderer = glGetString(GL_RENDERER);
    if (renderer)
    {
        if ((strstr((const char*)renderer,"Radeon")!=0) ||
            (strstr((const char*)renderer,"RADEON")!=0))
        {
            setFactorMultiplier(256.0f);
            setUnitMultiplier(256.0f);
            osg::notify(osg::INFO)<<"PolygonOffset::setFactorAndUnitMultipliersUsingBestGuessForDriver() apply ATI workaround."<<std::endl;
        }
    }
}


PolygonOffset::PolygonOffset():
    _factor(0.0f),
    _units(0.0f)
{
}

PolygonOffset::PolygonOffset(float factor, float units):
    _factor(factor),
    _units(units)
{
}

PolygonOffset::~PolygonOffset()
{
}

void PolygonOffset::apply(State&) const
{
    if (!s_MultiplerSet) setFactorAndUnitMultipliersUsingBestGuessForDriver();

    glPolygonOffset(_factor * s_FactorMultipler, 
                    _units * s_UnitMultipler);
}
