package require vtktcl
package require vtktcl_interactor
package require vtktcl_colors

# Create the RenderWindow, Renderer and both Actors
#
vtkRenderer ren1
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin

# create a semi-cylinder
#
vtkLineSource line
  line SetPoint1 0 1 0
  line SetPoint2 0 1 2
  line SetResolution 10

vtkRotationalExtrusionFilter lineSweeper
  lineSweeper SetResolution 20
  lineSweeper SetInput [line GetOutput]
  lineSweeper SetAngle 270

vtkBrownianPoints bump
  bump SetInput [lineSweeper GetOutput]

vtkWarpVector warp
  warp SetInput [bump GetPolyDataOutput]
  warp SetScaleFactor .2

vtkWindowedSincPolyDataFilter smooth
    smooth SetInput [warp GetPolyDataOutput]
    smooth SetNumberOfIterations 20
    smooth BoundarySmoothingOn
    smooth SetFeatureAngle 120
    smooth SetEdgeAngle 90
    smooth SetPassBand 0.1

vtkPolyDataNormals normals
    normals SetInput [smooth GetOutput]

vtkPolyDataMapper cylMapper
    cylMapper SetInput [normals GetOutput]

vtkActor cylActor
    cylActor SetMapper cylMapper
    eval [cylActor GetProperty] SetInterpolationToGouraud
    eval [cylActor GetProperty] SetInterpolationToFlat
    eval [cylActor GetProperty] SetColor $beige

vtkPolyDataMapper originalMapper
    originalMapper SetInput [bump GetPolyDataOutput]

vtkActor originalActor
    originalActor SetMapper originalMapper
    eval [originalActor GetProperty] SetInterpolationToFlat
    eval [cylActor GetProperty] SetColor $tomato

# Add the actors to the renderer, set the background and size
#
ren1 AddActor cylActor
#ren1 AddActor originalActor
ren1 SetBackground 1 1 1
renWin SetSize 200 300

vtkCamera camera
    camera SetClippingRange 0.576398 28.8199
    camera SetFocalPoint 0.0463079 -0.0356571 1.01993
    camera SetPosition -2.47044 2.39516 -3.56066
    camera SetViewUp 0.607296 -0.513537 -0.606195
ren1 SetActiveCamera camera

# render the image
#
iren SetUserMethod {wm deiconify .vtkInteract}

iren Initialize

# prevent the tk window from showing up then start the event loop
wm withdraw .





