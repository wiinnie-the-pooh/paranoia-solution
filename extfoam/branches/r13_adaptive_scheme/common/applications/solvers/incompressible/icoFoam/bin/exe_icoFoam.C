//  Copyright (C) 2009 Pebble Bed Modular Reactor (Pty) Limited (PBMR)
//
//  This code is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://sourceforge.net/projects/pyfoam
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
