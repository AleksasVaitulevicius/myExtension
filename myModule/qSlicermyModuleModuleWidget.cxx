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
#include <QMenu>
#include <QString>

// SlicerQt includes
#include "qSlicermyModuleModuleWidget.h"
#include "ui_qSlicermyModuleModuleWidget.h"

//#include "qSlicerReformatModuleWidget.h"

// SlicerQt includes
#include "qMRMLSliceControllerWidget_p.h" // For updateSliceOrientationSelector
#include "vtkMRMLSliceNode.h"

//// MRML includes
#include "vtkMRMLApplicationLogic.h"
// MRML includes
#include "vtkMRMLApplicationLogic.h"
#include "vtkMRMLCameraNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLSliceLogic.h"
#include "vtkMRMLVolumeNode.h"

// VTK includes
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkNew.h>
#include <vtkTransform.h>

// IO for printing to files
#include <iostream>
#include <fstream>
#include <string>
//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicermyModuleModuleWidgetPrivate: public Ui_qSlicermyModuleModuleWidget
{
	Q_DECLARE_PUBLIC(qSlicermyModuleModuleWidget);
protected:
	qSlicermyModuleModuleWidget* const q_ptr;

public:
  qSlicermyModuleModuleWidgetPrivate(qSlicermyModuleModuleWidget& object);

  vtkMRMLSliceNode* MRMLSliceNode;
  vtkMRMLSliceLogic* MRMLSliceLogic;
};

//-----------------------------------------------------------------------------
// qSlicermyModuleModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidgetPrivate::qSlicermyModuleModuleWidgetPrivate(qSlicermyModuleModuleWidget& object): q_ptr(&object)
{
	this->MRMLSliceNode = 0;
	this->MRMLSliceLogic = 0;
}

//-----------------------------------------------------------------------------
// qSlicermyModuleModuleWidget methods

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidget::qSlicermyModuleModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicermyModuleModuleWidgetPrivate(*this) )
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

	this->connect(d->SliceNodeSelector,
		SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		SLOT(onNodeSelected(vtkMRMLNode*)));

	this->connect(this,
		SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
		SLOT(onNewScene(vtkMRMLScene*)));

	this->connect(d->MyButton, SIGNAL(pressed()),
		this, SLOT(onLog()));
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onLog()
{
	ofstream outfile;

	outfile.open("log.txt", std::ios_base::app);
	outfile << "Hi!\n";
	outfile.close();
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onNewScene(vtkMRMLScene* scene)
{
	Q_D(qSlicermyModuleModuleWidget);

	d->SliceNodeSelector->setMRMLScene(scene);
	vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(
		scene->GetFirstNodeByClass("vtkMRMLSliceNode")
	);

	// Listen for SliceNode changes
	this->qvtkReconnect(d->MRMLSliceNode, sliceNode,
		vtkCommand::ModifiedEvent,
		this, SLOT(onMRMLSliceNodeModified(vtkObject*)));

	d->MRMLSliceNode = sliceNode;
	d->MRMLSliceLogic =
		this->logic()->GetMRMLApplicationLogic()->GetSliceLogic(d->MRMLSliceNode);
	
		ofstream outfile;

	outfile.open("log.txt", std::ios_base::app);
	outfile << "current node:" << sliceNode->GetNodeTagName() << "\n";
	outfile.close();
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

	ofstream outfile;

	outfile.open("log.txt", std::ios_base::app);
	outfile << "onNodeSelected()\n";
	outfile.close();
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onMRMLSliceNodeModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	ofstream outfile;

	outfile.open("log.txt", std::ios_base::app);
	outfile << "onMRMLSliceNodeModified()\n";
	outfile.close();

	vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(caller);
	if (!sliceNode)
	{
		return;
	}
}

//-----------------------------------------------------------
bool qSlicermyModuleModuleWidget::setEditedNode(vtkMRMLNode* node,
	QString role /* = QString()*/,
	QString context /* = QString()*/)
{
	Q_D(qSlicermyModuleModuleWidget);
	Q_UNUSED(role);
	Q_UNUSED(context);

	ofstream outfile;

	outfile.open("log.txt", std::ios_base::app);
	outfile << "setEditedNode:";

	if (vtkMRMLSliceNode::SafeDownCast(node))
	{
		d->SliceNodeSelector->setCurrentNode(node);
		outfile << "success no 1\n";
		outfile.close();
		return true;
	}

	if (vtkMRMLSliceCompositeNode::SafeDownCast(node))
	{
		vtkMRMLSliceCompositeNode* sliceCompositeNode = vtkMRMLSliceCompositeNode::SafeDownCast(node);
		vtkMRMLSliceNode* sliceNode = vtkMRMLSliceLogic::GetSliceNode(sliceCompositeNode);
		if (!sliceNode)
		{
			outfile << "failure no 1\n";
			outfile.close();
			return false;
		}
		d->SliceNodeSelector->setCurrentNode(sliceNode);
		outfile << "success no 2\n";
		outfile.close();
		return true;
	}

	outfile << "failure no 2\n";
	outfile.close();
	return false;
}