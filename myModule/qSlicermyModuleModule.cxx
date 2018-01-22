/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// myModule Logic includes
#include <vtkSlicermyModuleLogic.h>

// myModule includes
#include "qSlicermyModuleModule.h"
#include "qSlicermyModuleModuleWidget.h"

//-----------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtPlugin>
Q_EXPORT_PLUGIN2(qSlicermyModuleModule, qSlicermyModuleModule);
#endif

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicermyModuleModulePrivate
{
public:
  qSlicermyModuleModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicermyModuleModulePrivate methods

//-----------------------------------------------------------------------------
qSlicermyModuleModulePrivate::qSlicermyModuleModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicermyModuleModule methods

//-----------------------------------------------------------------------------
qSlicermyModuleModule::qSlicermyModuleModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicermyModuleModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicermyModuleModule::~qSlicermyModuleModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicermyModuleModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicermyModuleModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicermyModuleModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Aleksas Vaitulevicius aleksas1995@gmail.com");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicermyModuleModule::icon() const
{
  return QIcon(":/Icons/myModule.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicermyModuleModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicermyModuleModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicermyModuleModule
::createWidgetRepresentation()
{
  return new qSlicermyModuleModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicermyModuleModule::createLogic()
{
  return vtkSlicermyModuleLogic::New();
}
