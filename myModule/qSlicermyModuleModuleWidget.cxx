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

// utils
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>

//-----------------------------------------------------------------------------
class Interaction
{
public:
	enum Type { RED_INTERACTION, YELLOW_INTERACTION, GREEN_INTERACTION};
	Type interactionType;
	double value;

	Interaction(Type type, double value)
	{
		this->interactionType = type;
		this->value = value;
	}
};

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
  void perform(std::vector<Interaction> interactions);
  int getLogPosition(int position, std::ifstream* infile);
  std::vector<Interaction> toVector(std::ifstream* infile);

  vtkMRMLSliceNode* red;
  vtkMRMLSliceLogic* redLogic;

  vtkMRMLSliceNode* yellow;
  vtkMRMLSliceLogic* yellowLogic;

  vtkMRMLSliceNode* green;
  vtkMRMLSliceLogic* greenLogic;

  const std::string RED = "red_slice=";
  const std::string YELLOW = "yellow_slice=";
  const std::string GREEN = "green_slice=";
  const std::string FILE = "log.txt";

  int isRecording;
  int position;

private:
	void stepRed(double value);
	void stepYellow(double value);
	void stepGreen(double value);

	const std::time_t DELAY_OF_INTERACTION = 1;
	std::time_t lastInteraction;
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

	this->isRecording = 0;
	this->position = 0;
	this->lastInteraction = std::time(nullptr);
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidgetPrivate::recordSliceChange(double value, std::string type)
{
	std::ostringstream strs;
	strs << value;

	ofstream outfile;
	outfile.open(this->FILE, std::ios_base::app);

	std::time_t current = std::time(nullptr);
	std::time_t interval = difftime(current, this->lastInteraction);
	this->lastInteraction = current;

	if(interval >= this->DELAY_OF_INTERACTION)
		outfile << "Interaction\n";

	outfile << type << strs.str() << "\n";
	outfile.close();
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidgetPrivate::perform(std::vector<Interaction> interactions)
{
	for (auto const& interaction: interactions)
	{
		if (interaction.interactionType == Interaction::RED_INTERACTION)
		{
			stepRed(interaction.value);
			continue;
		}

		if (interaction.interactionType == Interaction::YELLOW_INTERACTION)
		{
			stepYellow(interaction.value);
			continue;
		}

		if (interaction.interactionType == Interaction::GREEN_INTERACTION)
		{
			stepGreen(interaction.value);
			continue;
		}
	}
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidgetPrivate::stepRed(double value)
{
	if (value == 0)
		return;

	this->redLogic->StartSliceOffsetInteraction();
	this->redLogic->SetSliceOffset(value);
	this->redLogic->EndSliceOffsetInteraction();
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidgetPrivate::stepYellow(double value)
{
	if (value == 0)
		return;

	this->yellowLogic->StartSliceOffsetInteraction();
	this->yellowLogic->SetSliceOffset(value);
	this->yellowLogic->EndSliceOffsetInteraction();
}

//-----------------------------------------------------------------------------
void qSlicermyModuleModuleWidgetPrivate::stepGreen(double value)
{
	if (value == 0)
		return;

	this->greenLogic->StartSliceOffsetInteraction();
	this->greenLogic->SetSliceOffset(value);
	this->greenLogic->EndSliceOffsetInteraction();
}

//------------------------------------------------------------------------------
int qSlicermyModuleModuleWidgetPrivate::getLogPosition(int position, std::ifstream* infile)
{
	std::string line;
	while (std::getline(*infile, line))
	{
		if (line.find("Interaction") != -1)
		{
			if (position == 0)
				return 1;
			position--;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
std::vector<Interaction> qSlicermyModuleModuleWidgetPrivate::toVector(std::ifstream* infile)
{
	std::vector<Interaction> result;
	std::string line;
	while (std::getline(*infile, line))
	{
		if (line.find("Interaction") != -1)
		{
			return result;
		}

		if (line.find(this->RED) != -1)
		{
			result.push_back(Interaction(Interaction::RED_INTERACTION, std::stod(line.substr(this->RED.size()))));
			continue;
		}

		if (line.find(this->YELLOW) != -1)
		{
			result.push_back(Interaction(Interaction::YELLOW_INTERACTION, std::stod(line.substr(this->YELLOW.size()))));
			continue;
		}

		if (line.find(this->GREEN) != -1)
		{
			result.push_back(Interaction(Interaction::GREEN_INTERACTION, std::stod(line.substr(this->GREEN.size()))));
			continue;
		}
	}

	return result;
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

	this->connect(d->StepForward, SIGNAL(pressed()),
		this, SLOT(stepForward()));
	this->connect(d->StepBackward, SIGNAL(pressed()),
		this, SLOT(stepBackward()));
	this->connect(d->StartRecording, SIGNAL(pressed()),
		this, SLOT(startRecording()));
	this->connect(d->StopRecording, SIGNAL(pressed()),
		this, SLOT(stopRecording()));
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onRedSliceModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	if (d->isRecording)
		d->recordSliceChange(d->redLogic->GetSliceOffset(), d->RED);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onYellowSliceModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	if (d->isRecording)
		d->recordSliceChange(d->yellowLogic->GetSliceOffset(), d->YELLOW);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::onGreenSliceModified(vtkObject* caller)
{
	Q_D(qSlicermyModuleModuleWidget);

	if (d->isRecording)
		d->recordSliceChange(d->greenLogic->GetSliceOffset(), d->GREEN);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::stepForward()
{
	Q_D(qSlicermyModuleModuleWidget);
	if (!d->redLogic || !d->yellowLogic || !d->greenLogic)
	{
		return;
	}

	int wasRecording = d->isRecording;

	d->isRecording = 0;

	std::ifstream infile(d->FILE);

	if (d->getLogPosition(d->position, &infile) == 0)
	{
		infile.close();
		return;
	}

	d->position++;

	std::vector<Interaction> interactions = d->toVector(&infile);

	d->perform(interactions);
	d->isRecording = wasRecording;
	infile.close();
	interactions.clear();
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::stepBackward()
{
	Q_D(qSlicermyModuleModuleWidget);
	if (!d->redLogic || !d->yellowLogic || !d->greenLogic || d->position == 0)
	{
		return;
	}

	int wasRecording = d->isRecording;

	d->isRecording = 0;

	int currentPosition = d->position;
	std::ifstream infile(d->FILE);

	if (d->getLogPosition(d->position - 1, &infile) == 0)
	{
		infile.close();
		return;
	}

	d->position--;

	std::vector<Interaction> interactions = d->toVector(&infile);
	std::reverse(interactions.begin(), interactions.end());

	d->perform(interactions);
	d->isRecording = wasRecording;
	infile.close();
	interactions.clear();
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::startRecording()
{
	Q_D(qSlicermyModuleModuleWidget);
	d->isRecording = 1;
	d->StopRecording->setEnabled(true);
	d->StartRecording->setEnabled(false);
}

//------------------------------------------------------------------------------
void qSlicermyModuleModuleWidget::stopRecording()
{
	Q_D(qSlicermyModuleModuleWidget);
	d->isRecording = 0;
	d->StopRecording->setEnabled(false);
	d->StartRecording->setEnabled(true);
}