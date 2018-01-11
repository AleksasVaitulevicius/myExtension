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

// SlicerQt includes
#include "qMRMLSliceControllerWidget_p.h" // For updateSliceOrientationSelector
#include "vtkMRMLSliceNode.h"

//#include "D:\S4\Modules\Loadable\Reformat\qSlicerReformatModuleWidget.h"

//// MRML includes
#include "vtkMRMLApplicationLogic.h"

// IO for printing to files
#include <iostream>
#include <fstream>
//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicermyModuleModuleWidgetPrivate: public Ui_qSlicermyModuleModuleWidget
{
public:
  qSlicermyModuleModuleWidgetPrivate();

  vtkMRMLSliceNode* MRMLSliceNode;
  vtkMRMLSliceLogic* MRMLSliceLogic;
};

//-----------------------------------------------------------------------------
// qSlicermyModuleModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidgetPrivate::qSlicermyModuleModuleWidgetPrivate()
{
	this->MRMLSliceNode = 0;
	this->MRMLSliceLogic = 0;
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

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::
onMRMLSliceNodeModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(caller);
	if (!sliceNode)
	{
		return;
	}

	ofstream myfile;
	myfile.open("log.txt");
	myfile << "Smthng updated.\n";
	myfile.close();
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onNodeSelected(vtkMRMLNode* node)
{
	Q_D(qSlicermyModuleModuleWidget);

	vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(node);

	// Listen for SliceNode changes
	this->qvtkReconnect(d->MRMLSliceNode, sliceNode,
		vtkCommand::ModifiedEvent,
		this, SLOT(onMRMLSliceNodeModified(vtkObject*)));

	d->MRMLSliceNode = sliceNode;
	d->MRMLSliceLogic =
		this->logic()->GetMRMLApplicationLogic()->GetSliceLogic(d->MRMLSliceNode);

	ofstream myfile;
	myfile.open("log.txt");
	myfile << "Smthng selected.\n";
	myfile.close();
}