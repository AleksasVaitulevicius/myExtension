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

#ifndef __qSlicermyModuleModuleWidget_h
#define __qSlicermyModuleModuleWidget_h

#include <QObject>

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicermyModuleModuleExport.h"

class qSlicermyModuleModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLScene;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_MYMODULE_EXPORT qSlicermyModuleModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicermyModuleModuleWidget(QWidget *parent=0);
  virtual ~qSlicermyModuleModuleWidget();

  virtual bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString());

public slots:

protected slots :
	void onLog();
	void onNewScene(vtkMRMLScene* scene);
	void onNodeSelected(vtkMRMLNode* node);
	void onMRMLSliceNodeModified(vtkObject* caller);

protected:
  QScopedPointer<qSlicermyModuleModuleWidgetPrivate> d_ptr;

  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicermyModuleModuleWidget);
  Q_DISABLE_COPY(qSlicermyModuleModuleWidget);
};

#endif
