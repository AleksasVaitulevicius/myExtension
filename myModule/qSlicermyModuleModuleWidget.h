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

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicermyModuleModuleExport.h"

class qSlicermyModuleModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_MYMODULE_EXPORT qSlicermyModuleModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicermyModuleModuleWidget(QWidget *parent=0);
  virtual ~qSlicermyModuleModuleWidget();

public slots:


protected:
  QScopedPointer<qSlicermyModuleModuleWidgetPrivate> d_ptr;
  void onNodeSelected(vtkMRMLNode* node);
  /// Triggered upon MRML transform node updates
  void onMRMLSliceNodeModified(vtkObject* caller);

  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicermyModuleModuleWidget);
  Q_DISABLE_COPY(qSlicermyModuleModuleWidget);
};

#endif
