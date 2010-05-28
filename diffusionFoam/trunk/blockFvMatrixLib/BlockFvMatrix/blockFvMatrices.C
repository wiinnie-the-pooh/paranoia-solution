/*-------------------------------------------------------------------------------------------------
    Copyright PBMR Pty Ltd.
    This file is a property of PBMR Pty Ltd, Centurion, Pretoria, South Africa. All rights reserved.
    No part of this source code maybe reproduced, stored in a retrieval system or transmitted in
    any form or by any means, electronic, mechanical, photocopying, recording or otherwise without
    the prior written permission of the company.
    Name:
        blockFvMatrices.C
    Description:
        blockFvMatrix static data members
-------------------------------------------------------------------------------------------------*/

#include "blockFvMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineNamedTemplateTypeNameAndDebug(blockVector2FvMatrix, 0);
defineNamedTemplateTypeNameAndDebug(blockVectorFvMatrix, 0);
defineNamedTemplateTypeNameAndDebug(blockVector4FvMatrix, 0);
defineNamedTemplateTypeNameAndDebug(blockVector6FvMatrix, 0);
defineNamedTemplateTypeNameAndDebug(blockVector8FvMatrix, 0);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
