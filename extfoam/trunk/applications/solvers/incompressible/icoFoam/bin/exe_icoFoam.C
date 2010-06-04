//  VulaSHAKA (Simultaneous Neutronic, Fuel Performance, Heat And Kinetics Analysis)
//  Copyright (C) 2009-2010 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  See https://vulashaka.svn.sourceforge.net/svnroot/vulashaka
//
//  Author : Alexey PETROV


//---------------------------------------------------------------------------

#include "ext_icoFoam.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    
    boost::shared_ptr< Foam::t_get_time_base > 
	a_get_time( new Foam::t_get_time( Foam::Time::controlDictName, args.rootPath(), args.caseName() ) );
	
    boost::shared_ptr< Foam::t_get_mesh_base > a_get_mesh( new Foam::t_get_mesh() );

    boost::shared_ptr< Foam::t_get_fields_base > a_get_fields( new Foam::t_get_fields() );

    boost::shared_ptr< Foam::t_read_piso_controls_base > a_read_piso_controls( new Foam::t_read_piso_controls() );

    Foam::icoFoam( a_get_time.get(), a_get_mesh.get(), a_get_fields.get() ).run( a_read_piso_controls.get() );

    return 0;
}


// ************************************************************************* //
