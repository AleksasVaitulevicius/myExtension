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

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicermyModuleModuleWidget.h"
#include "ui_qSlicermyModuleModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicermyModuleModuleWidgetPrivate: public Ui_qSlicermyModuleModuleWidget
{
public:
  qSlicermyModuleModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicermyModuleModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidgetPrivate::qSlicermyModuleModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicermyModuleModuleWidget methods

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidget::qSlicermyModuleModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicermyModuleModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidget::~qSlicermyModuleModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::setup()
{
  Q_D(qSlicermyModuleModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}
