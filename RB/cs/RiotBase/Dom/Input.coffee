InputMixin          = require '../Dom/Core/InputMixin.js'
ReactDOM            = require "react-dom"
React               = require "react"

#
module.exports = React.createClass
    mixins: [ InputMixin ]

    render: ->
        React.createElement "div", { style: { display: "inline-block", position: "relative", padding: "0px", margin: "0px" } }, React.createElement "input", @_input_props()

    _input_element: ->
        ReactDOM.findDOMNode( this ).getElementsByTagName( 'input' )[ 0 ]
