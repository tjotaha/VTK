from vtk import *

source = vtkRandomGraphSource()
source.SetNumberOfVertices(15)
source.SetStartWithTree(True)
source.SetIncludeEdgeWeights(True)

bfs = vtkBoostBreadthFirstSearch()
bfs.AddInputConnection(source.GetOutputPort())
bfs.SetOriginVertex(0)

view = vtkGraphLayoutView()
view.AddRepresentationFromInputConnection(bfs.GetOutputPort())
view.SetVertexLabelArrayName("BFS")
view.SetVertexLabelVisibility(True)
view.SetVertexColorArrayName("BFS")
view.SetColorVertices(True)
view.SetEdgeColorArrayName("edge weight")
view.SetColorEdges(False)
view.SetLayoutStrategyToSimple2D()
view.SetVertexLabelFontSize(20)

theme = vtkViewTheme.CreateMellowTheme()
theme.SetLineWidth(5)
theme.SetPointSize(10)
theme.SetCellOpacity(1)
theme.SetVertexLabelColor(0, 0, 0)
view.ApplyViewTheme(theme)
theme.FastDelete()


view.GetRenderWindow().SetSize(600, 600)
view.ResetCamera()
view.Render()

view.GetInteractor().Start()

