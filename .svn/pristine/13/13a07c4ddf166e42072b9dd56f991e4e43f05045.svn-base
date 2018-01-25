/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicermyModuleFooBarWidget.h"
#include "ui_qSlicermyModuleFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_myModule
class qSlicermyModuleFooBarWidgetPrivate
  : public Ui_qSlicermyModuleFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicermyModuleFooBarWidget);
protected:
  qSlicermyModuleFooBarWidget* const q_ptr;

public:
  qSlicermyModuleFooBarWidgetPrivate(
    qSlicermyModuleFooBarWidget& object);
  virtual void setupUi(qSlicermyModuleFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicermyModuleFooBarWidgetPrivate
::qSlicermyModuleFooBarWidgetPrivate(
  qSlicermyModuleFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicermyModuleFooBarWidgetPrivate
::setupUi(qSlicermyModuleFooBarWidget* widget)
{
  this->Ui_qSlicermyModuleFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicermyModuleFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicermyModuleFooBarWidget
::qSlicermyModuleFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicermyModuleFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicermyModuleFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicermyModuleFooBarWidget
::~qSlicermyModuleFooBarWidget()
{
}
