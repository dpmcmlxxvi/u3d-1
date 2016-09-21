
// First include the required header files for the VTK classes we are using.
#include "vtkCubeSource.h"
#include "vtkTubeFilter.h"
#include "vtkGeometryFilter.h"
#include "vtkElevationFilter.h"
#include "vtkPointDataToCellData.h"
#include "vtkAxes.h"
#include "vtkConeSource.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
//#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkVRMLExporter.h>
#include <vtkX3DExporter.h>
#include "vtkU3DExporter.h"
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkCullerCollection.h>
#include <vtkFloatArray.h>
#include <vtkVoxel.h>
#include <vtkHexahedron.h>
#include <vtkTetra.h>
#include <vtkWedge.h>
#include <vtkPyramid.h>
#include <vtkPixel.h>
#include <vtkQuad.h>
#include <vtkTriangle.h>
#include <vtkPolygon.h>
#include <vtkTriangleStrip.h>
#include <vtkLine.h>
#include <vtkPolyLine.h>
#include <vtkVertex.h>
#include <vtkPolyVertex.h>
#include <vtkPentagonalPrism.h>
#include <vtkHexagonalPrism.h>
#include <vtkAppendFilter.h>
#include <vtkTransformFilter.h>
#include <vtkTransform.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>


#define VTK_CREATE(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#define ADD_GRID(name, x, y, z, ...)                                                \
  double tmp##name [][3] = __VA_ARGS__;                                             \
  VTK_CREATE(vtkPoints, name##Points);                                              \
  name##Points->SetNumberOfPoints(sizeof(tmp##name)/sizeof(*tmp##name));            \
  for( unsigned int i=0; i < (sizeof(tmp##name)/sizeof(*tmp##name)); i++)           \
    name##Points->InsertPoint(i,tmp##name[i][0],tmp##name[i][1],tmp##name[i][2]);   \
                                                                                    \
  VTK_CREATE(vtk##name, a##name);                                                   \
  a##name->GetPointIds()->SetNumberOfIds(sizeof(tmp##name)/sizeof(*tmp##name));     \
  for( unsigned int i=0; i < (sizeof(tmp##name)/sizeof(*tmp##name)); i++)           \
    a##name->GetPointIds()->SetId(i,i);                                             \
                                                                                    \
  VTK_CREATE(vtkUnstructuredGrid, a##name##Grid);           \
  a##name##Grid->Allocate(1,1);                             \
  a##name##Grid->InsertNextCell(a##name->GetCellType(), a##name->GetPointIds()); \
  a##name##Grid->SetPoints(name##Points);                   \
                                                            \
  VTK_CREATE(vtkTransform, transform##name);                \
  transform##name->Translate(x, y, z);                      \
  VTK_CREATE(vtkTransformFilter, trans##name##Filter);      \
  trans##name##Filter->SetInputData(a##name##Grid);             \
  trans##name##Filter->SetTransform(transform##name);       \
  appendF->AddInputData(trans##name##Filter->GetOutput());      \
                                                            \
  VTK_CREATE(vtkDataSetMapper, a##name##Mapper);            \
  a##name##Mapper->SetInputData(a##name##Grid);                 \
                                                            \
  vtkActor *a##name##Actor = vtkActor::New();               \
  a##name##Actor->SetMapper(a##name##Mapper);               \
  a##name##Actor->AddPosition(x, y, z);                     \
  a##name##Actor->GetProperty()->BackfaceCullingOn();


int main(int argc, char *argv[]) {

    VTK_CREATE(vtkUnstructuredGrid, aGrid);
    VTK_CREATE(vtkAppendFilter, appendF);

// Voxel
    ADD_GRID(Voxel, 0, 0, 0, {
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 1, 1, 0 },
        { 0, 0, 1 },
        { 1, 0, 1 },
        { 0, 1, 1 },
        { 1, 1, 1 },
    });

    vtkRenderer *ren1 = vtkRenderer::New();
    // turn off all cullers
    ren1->GetCullers()->RemoveAllItems();

    ren1->AddActor(aVoxelActor);
    aVoxelActor->GetProperty()->SetDiffuseColor(1, 0, 0);

    VTK_CREATE(vtkLight, pointLight);
    pointLight->SetFocalPoint(7.0, 2.0, 2.0);
    pointLight->SetPosition(7, 2, 20);
    pointLight->SetColor(1, 1, 1);
    pointLight->PositionalOn();
    pointLight->SetConeAngle(170);
    ren1->AddLight(pointLight);

    VTK_CREATE(vtkLight, dirLight);
    dirLight->SetFocalPoint(7.0, 2.0, 0.0);
    dirLight->SetPosition(7, 2.5, -20);
    dirLight->SetColor(1, 1, 1);
    dirLight->PositionalOff();
    ren1->AddLight(dirLight);

    ren1->SetBackground(0.1, 0.2, 0.4);
    ren1->GetActiveCamera()->SetPosition(7.0, 2.0, 20.0);
    ren1->GetActiveCamera()->SetFocalPoint(7.0, 2.0, 2.0);
    ren1->GetActiveCamera()->SetViewAngle(45.0);

    //
    // Finally we create the render window which will show up on the screen.
    // We put our renderer into the render window using AddRenderer. We also
    // set the size to be 300 pixels by 300.
    //
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(ren1);
    renWin->SetSize(600, 600);

//    renWin->Render();
//
//    VTK_CREATE(vtkWindowToImageFilter, windowToImage);
//    windowToImage->SetInput(renWin);

//    VTK_CREATE(vtkPNGWriter, PNGWriter);
//    PNGWriter->SetInputConnection(windowToImage->GetOutputPort());
//    PNGWriter->SetFileName("test.png");
//    PNGWriter->Write();
//
//    VTK_CREATE(vtkVRMLExporter, vrmlExp);
//    vrmlExp->SetRenderWindow(renWin);
//    vrmlExp->SetFileName("test.wrl");
//    vrmlExp->SetSpeed(5.5);
//    vrmlExp->Write();
//
//    VTK_CREATE(vtkX3DExporter, x3dExp);
//    x3dExp->SetRenderWindow(renWin);
//    x3dExp->SetFileName("test.x3d");
//    x3dExp->Update();
//    x3dExp->Write();

    VTK_CREATE(vtkU3DExporter, u3dExp);
    u3dExp->SetRenderWindow(renWin);
    u3dExp->SetFileName("test");
    u3dExp->SetMeshCompression(false);
    u3dExp->Write();

    //
    // The vtkRenderWindowInteractor class watches for events (e.g., keypress,
    // mouse) in the vtkRenderWindow. These events are translated into
    // event invocations that VTK understands (see VTK/Common/vtkCommand.h
    // for all events that VTK processes). Then observers of these VTK
    // events can process them as appropriate.
//    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
//    iren->SetRenderWindow(renWin);

    //
    // By default the vtkRenderWindowInteractor instantiates an instance
    // of vtkInteractorStyle. vtkInteractorStyle translates a set of events
    // it observes into operations on the camera, actors, and/or properties
    // in the vtkRenderWindow associated with the vtkRenderWinodwInteractor.
    // Here we specify a particular interactor style.
//    vtkInteractorStyleTrackballCamera *style =
//            vtkInteractorStyleTrackballCamera::New();
//  iren->SetInteractorStyle(style);

    //
    // Unlike the previous scripts where we performed some operations and then
    // exited, here we leave an event loop running. The user can use the mouse
    // and keyboard to perform the operations on the scene according to the
    // current interaction style. When the user presses the "e" key, by default
    // an ExitEvent is invoked by the vtkRenderWindowInteractor which is caught
    // and drops out of the event loop (triggered by the Start() method that
    // follows.
    //
//    iren->Initialize();
//    iren->Start();

    //
    // Final note: recall that an observers can watch for particular events and
    // take appropriate action. Pressing "u" in the render window causes the
    // vtkRenderWindowInteractor to invoke a UserEvent. This can be caught to
    // popup a GUI, etc. So the Tcl Cube5.tcl example for an idea of how this
    // works.

    //
    // Free up any objects we created. All instances in VTK are deleted by
    // using the Delete() method.
    //
    ren1->Delete();
    renWin->Delete();
//    iren->Delete();
//    style->Delete();

    return 0;
}


