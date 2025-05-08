import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
	id: resultsPanel
	SplitView.fillWidth: true

	HorizontalHeaderView {
		id: itemsHeader
		clip: true
		syncView: itemsView
		Layout.fillWidth: true
	}

	TreeView {
		id: itemsView
		clip: true
		boundsMovement: Flickable.StopAtBounds

		Layout.fillWidth: true
		Layout.fillHeight: true

		ScrollBar.horizontal: ScrollBar {}
		ScrollBar.vertical: ScrollBar {}

		//model: Acquisition.searchResultsModel

		delegate: TreeViewDelegate {
			id: itemViewDelegate
			contentItem: Label {
				text: itemViewDelegate.model.display
			}
		}
	}
}
