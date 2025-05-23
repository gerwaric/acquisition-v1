import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.2
import QtQml 2.12 // Provides access to Qt.inputMethod.
//import org.kde.kirigami 2.10 as Kirigami
//import local 1.0 as Local // Our custom QML components, as exported in main.cpp.

// Auto-complete widget with completion highlighting similar to a Google Search.
//
//   When using this, be sure to adjust the z indexes of sibling items so that the container of the
//   autocomplete widget (e.g. RowLayout) has a higher z value than the container of every widget
//   following it that might be overlaid by the suggestions.
//
//   TODO: To avoid having to adjust z indexes when using this, perhaps place the suggestions into
//   a Kirigami Sheet, or use the same mechanism to overlay a target widget. Or introduce a property
//   to reference the component to overlay, and then do Component.onCompleted: { while
//   (notOverlaid(targetToOverlay)) { ancestor = ancestor.parent; ancestor.z += 1000 }.
//
//   TODO: Set a minimum width. Otherwise, without AutoComplete { Layout.fillWidth: true },
//   the auto-complete box would not be visible at all.
//
//   TODO: Fix that the parts of this auto-complete widget have confusing internal
//   dependencies. For example, after changing the input property one also always has to
//   do completions.currentIndex = -1. That should be handled automatically.
//
//   TODO: Clear up the confusion between an invisible suggestion box and a visible one with
//   no content. Both cases are used currently, but only one is necessary, as both look the same.
//
//   TODO: Use custom properties as a more declarative way to govern behavior here.
//   For example, "completionsVisibility" set to an expression. See:
//   https://github.com/dant3/qmlcompletionbox/blob/41eebf2b50ef4ade26c99946eaa36a7bfabafef5/SuggestionBox.qml#L36
//   https://github.com/dant3/qmlcompletionbox/blob/master/SuggestionBox.qml
//
//   TODO: Use states to better describe the open and closed state of the completions box.
//   See: https://code.qt.io/cgit/qt/qtdeclarative.git/tree/examples/quick/keyinteraction/focus/focus.qml?h=5.15#n166
FocusScope {
    id: autocomplete

    // Always give a QML component a defined height.
    //   The height is 0 by default, *not* the height of childrens. When not defining one here,
    //   the autocomplete will overlap other widgets following it in a ColumnLayout because it has
    //   height 0 and shows overflow. Defining height in the two ways below allows client code to
    //   place the widget into a Layout or a non-widget object. See: https://stackoverflow.com/a/38511223
    Layout.preferredHeight: field.height
    height: field.height

    // Data source containing the current autocomplete suggestions.
    //   This property must be set when instantiating this QML component. Acceptable models are string
    //   list models only. In JavaScript, these are a string array, so access it with [index], not the usual
    //   .get(index). To implement such a model in C++, use QStringList.
    //
    //   TODO: Allow client code of this component to set this property to any type of model accepted by a
    //   Repeater. See: https://doc.qt.io/qt-5/qml-qtquick-repeater.html#model-prop . Right now, only a
    //   QStringList is accepted.
    property alias model: completions.model

    // The currently active, in-use user input that is the basis for the current completions.
    //   It differs from the text field's "text" content while navigating through autocompletions.
    //   During that time, the last input as typed by the user is "input" and the text field content is
    //   a potential next input, with completion highlighting done based on the current input.
    //
    //   Use the `onInputChanged` handler to update the model providing the completions to the
    //   autocomplete. For example, if the model is the result of a database query, then the database
    //   query has to be run again to update it.
    //
    //   TODO: If it is necessary to also keep the "text" property, better rename the "input" property
    //   to "searchTerm". Because it's confusing otherwise.
    property string input

    // The current textual content of the text field.
    //   When the input property changes, the text property changes. But when the text property changes,
    //   the input property changes only if this was triggered by user input.
    //
    //   TODO: Probably, this does not have to be part of the public interface. If not, remove it,
    //   because the distinction between "input" and "text" is quite confusing.
    property alias text: field.text

    // The text shown in light gray in an empty autocomplete field.
    property alias placeholderText: field.placeholderText

    // If the box with completions is visible below the autocomplete field.
    property alias completionsVisible: completionsBox.visible

    // This signal is emitted when the Return or Enter key is pressed in the autocomplete's underlying
    // text field, or a completion is accepted with a mouse click or by pressing Return or Enter when
    // it is selected.
    signal accepted()

    // React to our own auto-provided signal for a change in the "input" property.
    //   When client code also implements onInputChanged when instantiating an AutoComplete, it
    //   will not overwrite this handler but add to it. So no caveats when reacting to own signals.
    //
    //   TODO: Make sure that text is only set here and not also unnecessarily in other locations in
    //   this component.
    onInputChanged: {
        console.log("AutoComplete: autocomplete: 'inputChanged()' signal received")
        text = input
    }

    // Normalize user input to a form based on which the completions can be calculated.
    //   Only a do-nothing implementation is provided. Client code should overwrite this as required
    //   by the model used.
    function normalize(input) { return input }

    // Highlight the auto-completed parts of a multi-substring search term using HTML.
    //
    // This implementation uses <b> tags to highlight the completed portions. Client code can
    // overwrite this to implement a different type of completion.
    //
    // @param fragmentsString  The part to not render in bold, when matched case-insensitively against the
    //   completion.
    //
    // TODO: Document the parameters.
    // TODO: Make sure original contains no HTML tags by sanitizing these. Otherwise searching
    //   for parts of original below may match "word</b>" etc. and mess up the result.
    // TODO: Add a parameter to make it configurable which HTML tag (incl. attributes) to use for the
    //   highlighting.
    // TODO: Maybe add a parameter to de-highlight the original parts with configurable HTML.
    // TODO: It seems better to do this in C++, as it will be much faster in regex processing etc..
    //   And it seems to be the responsibility of the model, and all models of this application are
    //   done in C++ as a convention.
    function highlightCompletion(completion, fragmentsString) {
        var fragments = fragmentsString.trim().split(" ")

        // Tracks the current processing position in "completion".
        //   This ensures that fragments are found in their given sequence. It also prevents
        //   modifying the <b> HTML tags themselves by matching against a "b" fragment. We'll simply
        //   work only on the part that has not yet had any insertions of HTML tags.
        var searchPos = 0

        // De-bold the sequential first occurrences of the fragments.
        for (var i = 0; i < fragments.length; i++) {
            var iCompletion = completion.toLowerCase() // "i" as in "case-Insensitive"
            var fragment = fragments[i]
            var fragmentStart = iCompletion.indexOf(fragment.toLowerCase(), searchPos)

            // Nothing to de-bolden if fragment was not found.
            //   TODO: This is an error condition that should generate a warning.
            if (fragmentStart === -1)
                continue

            var fragmentEnd = fragmentStart + fragment.length
            var completionBefore = completion.substr(0, fragmentStart)
            var completionDebold = completion.substr(fragmentStart, fragment.length)
            var completionAfter = completion.substr(fragmentEnd)

            completion = completionBefore + "</b>" + completionDebold + "<b>" + completionAfter

            // The search for the next fragment may start with completionAfter. That's after all HTML
            // tags inserted so far, which prevents messing them up when a "b" fragment matches against them.
            searchPos = completion.length - completionAfter.length - 1
        }

        // Wrap everything in bold to provide the proper "context" for the de-boldening above.
        //   This could not be done before to prevent matches of a "b" fragment against </b>".
        completion = "<b>" + completion + "</b>"

        return completion
    }

    // The text field where a user enters to-be-completed text.
    TextField {
        id: field
        focus: true

        // Our parent "Item {}" is not a layout, so we can't use "Layout.fillWidth: true".
        anchors.left: parent.left
        anchors.right: parent.right

        // Disable predictive text input to make textEdited() signals work under Android.
        //   A workaround for multiple Qt bugs. See: https://stackoverflow.com/a/62526369
        inputMethodHints: Qt.ImhSensitiveData

        //TBD selectByMouse: !Kirigami.Settings.isMobile

        // Necessary to have a blinking cursor in text field at application startup.
        //   Not done on mobile platforms because it would show the onscreen keyboard at startup,
        //   hiding half of the window.
        //
        //   TODO: Put the focus-at-startup management code in one place. Currently, the same line
        //   is needed in root.Component.onCompleted in App.qml. Perhaps a FocusScope can help.
        //   The auto-complete has now become a FocusScope, so maybe this solves it already?
        //TBD Component.onCompleted: if (!Kirigami.Settings.isMobile) forceActiveFocus()

        // This event handler is undocumented for TextField and incompletely documented for TextInput,
        // which TextField wraps: https://doc.qt.io/qt-5/qml-qtquick-textinput.html#textEdited-signal .
        // However, it works, and is also proposed by code insight in Qt Creator.
        onTextEdited: {
            // Update the current input because the user changed the text.
            //   User changes include cutting and pasting. The "textChanged()" event however
            //   is emitted also when software changes the text field content (such as when
            //   navigating through completions), making it the wrong place to update the input.
            //   This automatically emits inputChanged() so client code can adapt the model.
            autocomplete.input = text

            // Invalidate the completion selection, because the user edited the input so
            // it does not correspond to any current completion. Also completions might have been cleared.
            //   TODO: Probably better implement this reactively via onModelChanged, if there is such a thing.
            completions.currentIndex = -1

            completionsVisible = completions.model.length > 0 ? true : false;
        }

        // Handle the "text accepted" event, which sets the input from the text.
        //   This event is also artificially emitted by the "Search" button and by clicking on
        //   an auto-suggest proposal.
        //
        //   This event is emitted by a TextField when the user finishes editing it.
        //   In desktop software, this requires pressing "Return". Moving focus does not count.
        onAccepted: {
            console.log("AutoComplete: field: 'accepted()' received")

            // Give up the focus, making it available for grabs by the rest of the UI
            // via appropriate focus property bindings.
            autocomplete.focus = false

            autocomplete.completionsVisible = false
            // When clicking into the text field again, the last set of completions should show
            // again. But selecting them will start anew.
            //   TODO: Probably better implement this reactively via onModelChanged, if there is such a thing.
            completions.currentIndex = -1

            // True to the browser paradigm where URLs are fixed up, we'll correct the input entered.
            // Such as: " 2 165741  004149  " → "2165741004149"
            //TBD var searchTerm = normalize(text)
            autocomplete.input = text //TBD searchTerm

            autocomplete.accepted()
        }

        onActiveFocusChanged: {
            if (activeFocus && completions.model.length > 0)
                completionsVisible = (text == "" || text.match("^[0-9 ]+$")) ? false : true
                // TODO: Probably better use "input" instead of "text" in the line above.
                // TODO: Perhaps initialize the completions with suggestions based on the current
                // text. If the reason for not having the focus before was a previous
                // search, then it has no completions at this point.
            else
                completionsVisible = false
        }

        // Process all keyboard events here centrally.
        //   Since the TextEdit plus suggestions box is one combined component, handling all
        //   key presses here is more tidy. They cannot all be handled in completionsBox as
        //   key presses are not delivered or forwarded to components in their invisible state.
        Keys.onPressed: {
            console.log("AutoComplete: field: Keys.pressed(): " + event.key + " : " + event.text)

            if (completionsVisible) {
                switch (event.key) {

                case Qt.Key_Escape:
                    completionsVisible = false
                    completions.currentIndex = -1
                    event.accepted = true
                    break

                case Qt.Key_Up:
                    completions.currentIndex--

                    // When moving prior the first item, cycle through completions from the end again.
                    if (completions.currentIndex < 0)
                        completions.currentIndex = completions.model.length - 1

                    console.log(
                        "completions.model[" + completions.currentIndex + "]: " +
                        JSON.stringify(completions.model[completions.currentIndex])
                    )

                    field.text = completions.model[completions.currentIndex]
                    event.accepted = true
                    break

                case Qt.Key_Down:
                    completions.currentIndex++

                    // When moving past the last item, cycle through completions from the start again.
                    if (completions.currentIndex > completions.model.length - 1)
                        completions.currentIndex = 0

                    field.text = completions.model[completions.currentIndex]
                    event.accepted = true
                    break

                case Qt.Key_Return:
                    field.accepted()
                    event.accepted = true;
                    break
                }
            }
            else {
                switch (event.key) {

                // This way, "double Escape" can be used to move the focus to the browser.
                // The first hides the suggestions box, the second moves the focus.
                case Qt.Key_Escape:
                    // Give up the focus, making it available for grabs by the rest of the UI
                    // via appropriate focus property bindings.
                    autocomplete.focus = false
                    event.accepted = true
                    break

                case Qt.Key_Down:
                    completionsVisible = completions.model.length > 0 ? true : false

                    event.accepted = true
                    break
                }
            }
        }

        // Event handler for the non-instantiable QML object "Qt" resp. "Qt.inputMethod".
        //   The details of detecting keyboard changes like this: https://stackoverflow.com/a/62986064
        Connections {
            target: Qt.inputMethod

            // Hide the autocomplete's completions when the user hides the Android keyboard.
            //   Because it probably means that the user wants to see the browser, not the
            //   completions box. So we hide the completions box by removing the focus. We could
            //   also keep the focus, but focus is useless without a keyboard.
            onKeyboardRectangleChanged: {
                // Contains QML Basic Type "rect", automatically converted from Qt QRectF as per
                // https://doc.qt.io/qt-5/qml-rect.html
                var newRect = Qt.inputMethod.keyboardRectangle

                if (Qt.inputMethod.keyboardRectangle.height === 0) {
                    autocomplete.focus = false
                }
            }
        }

        // Custom mouse event handler that will show the completions box when clicking into the
        // text field. It is the complementary action to pressing "Esc" once, and does the same as
        // pressing "Arrow Down" while completionsBox is hidden.
        //
        //   Due to a bug in Qt, "propagateComposedEvents: true" has no effect when used inside a
        //   TextField in a StackView page (see https://forum.qt.io/topic/64041 ). There is a
        //   workaround in C++, but it is complex (see https://forum.qt.io/post/312884 ). Since this
        //   is non-essential behavior, we better watit for Qt to fix the bug.
        //
        //   TODO: Enable this code once Qt fixed the bug described above.
//      MouseArea {
//          anchors.fill: parent
//          // By propagating events and not accepting them in the handlers, the parent TextEdit can
//          // also react to them to set focus etc.. Source: https://stackoverflow.com/a/29765628
//          // propagateComposedEvents: true
//
//          onClicked: {
//              console.log("AutoComplete: field: clicked() received")
//              completionsVisible = completions.model.length > 0 ? true : false
//              mouse.accepted = false
//          }
//          // onPressed:         mouse.accepted = false
//          onDoubleClicked:   mouse.accepted = false
//          onPressAndHold:    mouse.accepted = false
//      }

        // Autocomplete dropdown.
        //   Using Rectangle{Column{Repeater}} here because a ListView does not support setting its
        //   height to its content's height because it's meant to be scrolled. Doesn't work even
        //   with "height: childrenRect.height". The Rectangle is just to provide a background canvas.
        //
        //   TODO: Make this a sub-component, means provide a public interface of (alias) properties
        //   and signals that is then accessed by the rest of the AutoComplete code. That avoids
        //   the confusing parallel use of "completionsBox" and "completions".
        Rectangle {
            id: completionsBox

            visible: false // Will be made visible once starting to type a category name.

            anchors.top: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: childrenRect.height

            color: "white" // The default, anyway.
            border.width: 1
            border.color: "silver" // TODO: Replace with the themed color used for field borders etc..

            Column {
                Repeater {
                    id: completions

                    // Repeater lacks ListView's currentIndex, so we'll add it.
                    property int currentIndex: -1 // No element highlighted initially.

                    // A delegate renders one list item.
                    //   TODO: Use a basic QML component to not tie AutoComplete to Kirigami. Or
                    //   document what can be used here when wanting to use it independent of Kirigami.
                    delegate: Label {
                        id: listItem

                        text: highlightCompletion(modelData, autocomplete.input)
                        width: completionsBox.width
                        //TBD reserveSpaceForIcon: false

                        // Background coloring should be used only for the selected item.
                        //   (Also, a lighter colored background automatically appears on mouse-over.)
                        highlighted: index == completions.currentIndex

                        onClicked: {
                            console.log("modelData = " + JSON.stringify(modelData))
                            completions.currentIndex = index
                            field.text = modelData
                            field.accepted()
                        }
                    }
                }
            }
        }
    }
}
