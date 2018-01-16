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

//#include "qSlicerReformatModuleWidget.h"

// SlicerQt includes
#include "qMRMLSliceControllerWidget_p.h" // For updateSliceOrientationSelector
#include "vtkMRMLSliceNode.h"
#include "vtkSlicermyModuleLogic.h"

//// MRML includes
#include "vtkMRMLApplicationLogic.h"
// MRML includes
#include "vtkMRMLApplicationLogic.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLSliceLogic.h"

// IO for printing to files
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicermyModuleModuleWidgetPrivate: public Ui_qSlicermyModuleModuleWidget
{
	Q_DECLARE_PUBLIC(qSlicermyModuleModuleWidget);
protected:
	qSlicermyModuleModuleWidget* const q_ptr;

public:
  qSlicermyModuleModuleWidgetPrivate(qSlicermyModuleModuleWidget& object); 
  void recordSliceChange(double value, std::string type);

  vtkMRMLSliceNode* red;
  vtkMRMLSliceLogic* redLogic;

  vtkMRMLSliceNode* yellow;
  vtkMRMLSliceLogic* yellowLogic;

  vtkMRMLSliceNode* green;
  vtkMRMLSliceLogic* greenLogic;

  const std::string RED = "red_slice=";
  const std::string YELLOW = "yellow_slice=";
  const std::string GREEN = "green_slice=";
  int isRunning;

private:
	const std::string FILE = "log.txt";
};

//-----------------------------------------------------------------------------
// qSlicermyModuleModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicermyModuleModuleWidgetPrivate::qSlicermyModuleModuleWidgetPrivate(qSlicermyModuleModuleWidget& object): q_ptr(&object)
{
	this->red = 0;
	this->redLogic = 0;

	this->yellow = 0;
	this->yellowLogic = 0;

	this->green = 0;
	this->greenLogic = 0;

	this->isRunning = 0;
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidgetPrivate::recordSliceChange(double value, std::string type)
{
	std::ostringstream strs;
	strs << value;

	ofstream outfile;

	outfile.open(this->FILE, std::ios_base::app);
	outfile << type << strs.str() << "\n";
	outfile.close();
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

	vtkSlicermyModuleLogic* logic = vtkSlicermyModuleLogic::SafeDownCast(this->logic());

	std::vector< vtkMRMLNode *> nodes;
	int nodesSize = logic->GetMRMLScene()->GetNodesByClass("vtkMRMLSliceNode", nodes);

	// Listen for SliceNode changes
	this->qvtkReconnect(d->red, vtkMRMLSliceNode::SafeDownCast(nodes[0]),
		vtkCommand::ModifiedEvent,
		this, SLOT(onRedSliceModified(vtkObject*)));

	this->qvtkReconnect(d->yellow, vtkMRMLSliceNode::SafeDownCast(nodes[1]),
		vtkCommand::ModifiedEvent,
		this, SLOT(onYellowSliceModified(vtkObject*)));

	this->qvtkReconnect(d->green, vtkMRMLSliceNode::SafeDownCast(nodes[2]),
		vtkCommand::ModifiedEvent,
		this, SLOT(onGreenSliceModified(vtkObject*)));

	d->red = vtkMRMLSliceNode::SafeDownCast(nodes[0]);
	d->redLogic = this->logic()->GetMRMLApplicationLogic()->GetSliceLogic(d->red);

	d->yellow = vtkMRMLSliceNode::SafeDownCast(nodes[1]);
	d->yellowLogic = this->logic()->GetMRMLApplicationLogic()->GetSliceLogic(d->yellow);

	d->green = vtkMRMLSliceNode::SafeDownCast(nodes[2]);
	d->greenLogic = this->logic()->GetMRMLApplicationLogic()->GetSliceLogic(d->green);

	this->connect(d->RunFromLog, SIGNAL(pressed()),
		this, SLOT(runFromLog()));
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onRedSliceModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	if (d->isRunning)
		return;

	d->recordSliceChange(d->redLogic->GetSliceOffset(), d->RED);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onYellowSliceModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	if (d->isRunning)
		return;

	d->recordSliceChange(d->yellowLogic->GetSliceOffset(), d->YELLOW);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onGreenSliceModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	if (d->isRunning)
		return;

	d->recordSliceChange(d->greenLogic->GetSliceOffset(), d->GREEN);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::runFromLog()
{
	Q_D(qSlicermyModuleModuleWidget);
	if (!d->redLogic || !d->yellowLogic || !d->greenLogic)
	{
		return;
	}

	d->isRunning = 1;

	std::ifstream infile("log.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		if (line.find(d->RED) != -1)
		{
			double val = std::stod(line.substr(d->RED.size()));

			if (val == 0)
				continue;

			d->redLogic->StartSliceOffsetInteraction();
			d->redLogic->SetSliceOffset(val);
			d->redLogic->EndSliceOffsetInteraction();
			continue;
		}

		if (line.find(d->YELLOW) != -1)
		{
			double val = std::stod(line.substr(d->YELLOW.size()));

			if (val == 0)
				continue;

			d->yellowLogic->StartSliceOffsetInteraction();
			d->yellowLogic->SetSliceOffset(val);
			d->yellowLogic->EndSliceOffsetInteraction();
			continue;
		}

		if (line.find(d->GREEN) != -1)
		{
			double val = std::stod(line.substr(d->GREEN.size()));

			if (val == 0)
				continue;

			d->greenLogic->StartSliceOffsetInteraction();
			d->greenLogic->SetSliceOffset(val);
			d->greenLogic->EndSliceOffsetInteraction();
			continue;
		}
	}

	d->isRunning = 0;
}