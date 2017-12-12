InputMixin          = require '../Dom/Core/InputMixin.js'

TextField           = require 'material-ui/lib/text-field'
ReactDOM            = require "react-dom"
React               = require "react"

#
module.exports = React.createClass
    mixins: [ InputMixin ]

    render: ->
        React.createElement TextField, @_input_props()

    _input_element: ->
        ReactDOM.findDOMNode( this ).getElementsByTagName( 'input' )[ 0 ]

# CaretCoordinates    = require './Core/CaretCoordinates.js' # "textarea-caret-position"
# String              = require './../Variable/String.js'
#
# ReactInputSelection = require 'react/lib/ReactInputSelection.js'
# TextField           = require 'material-ui/lib/text-field'
# ReactDOM            = require "react-dom"
# React               = require "react"
#
# #
# module.exports = React.createClass
#     propTypes:
#         obj: ( props, propName, componentName ) ->
#             if props.obj not instanceof String
#                 new Error 'Validation failed!'
#
#     getInitialState: ->
#         @_init_inst()
#         value: ''
#
#     render: ->
#         # make the props for the text_field propertis
#         React.createElement TextField, Object.assign {}, @aprops, @props, { value: @state.value }
#
#     componentWillUnmount: ->
#         @props.obj.rem_cursor @cursor_id
#         @props.obj.rm_onchange @onchange_id
#
#     _init_inst: ->
#         # make a new (inactive) cursor
#         @cursor_id  = @props.obj.new_cursor [ [ 0, @props.obj.length.val, 0 ] ]
#         @cursor     = @props.obj.get_cursor @cursor_id
#         @rect_map   = {}
#
#         # change in obj => change in the displayed text + cursor position
#         @onchange_id = @props.obj.onchange =>
#             @setState { value: @props.obj.val }, ->
#                 # after react rendering (real DOM elements), we can access to the input element
#                 # in order to render the selection(s)
#                 inp = @_input_element()
#                 usd = []
#                 for cid in @props.obj.get_cursor_ids()
#                     c = @props.obj.get_cursor( cid.loc_id, cid.dev_id ).val[ 0 ]
#                     if c[ 2 ] # if active
#                         if cid.dev_id.loc() and cid.loc_id == @cursor_id
#                             ReactInputSelection.setSelection inp, start: Math.min( c[ 0 ], c[ 1 ] ), end: Math.max( c[ 0 ], c[ 1 ] )
#                         else
#                             # do not remove the display for this cursor
#                             usd.push cid.dev_id.val + cid.loc_id
#
#                             @caretCoordinates or= new CaretCoordinates inp
#                             p0 = @caretCoordinates.get c[ 0 ]
#                             p1 = @caretCoordinates.get c[ 1 ]
#
#                             # prepare an object for the display
#                             rect = @rect_map[ cid.dev_id.val + cid.loc_id ]
#                             if not rect
#                                 rect = document.createElement 'span'
#                                 rect.style.borderWidth = 1
#                                 rect.style.borderColor = 'red'
#                                 rect.style.borderStyle = 'solid'
#                                 rect.style.position = 'absolute'
#                                 rect.style.height = p0.fontSize
#                                 rect.style.width = 2
#
#                                 @rect_map[ cid.dev_id.val + cid.loc_id ] = rect
#                                 ReactDOM.findDOMNode( this ).appendChild rect # document.body
#
#                             # update display
#                             rect.style.top   = Math.min( p0.top ,  p1.top  ) + 'px'
#                             rect.style.left  = Math.min( p0.left,  p1.left ) + 'px'
#                             rect.style.width = Math.abs( p1.left - p0.left ) + 'px'
#
#                 # remove unused ext selection
#                 for n, rect of @rect_map
#                     if n not in usd
#                         rect.remove()
#                         delete @rect_map[ n ]
#
#         # attribute for input item
#         @aprops =
#             onKeyDown: ( e ) =>
#                 switch e.keyCode
#                     when 8 # backspace
#                         npc = @cursor.val[ 0 ][ 0 ]
#                         npd = @cursor.val[ 0 ][ 1 ]
#                         if npc != npd
#                             beg = Math.min npc, npd
#                             end = Math.max npc, npd
#                             @props.obj.remove beg, end - beg
#                         else if npc
#                             if e.ctrlKey
#                                 str = @props.obj.val
#                                 while npc and str[ npc - 1 ] in " \t\n"
#                                     npc -= 1
#                                 while npc and str[ npc - 1 ] not in " \t\n"
#                                     npc -= 1
#                             else
#                                 npc -= 1
#                             @props.obj.remove npc, @cursor.val[ 0 ][ 0 ] - npc
#                         e.preventDefault()
#                     when 35 # end
#                         @cursor.val = [ [ @props.obj.length, @props.obj.length, 1 ] ]
#                         e.preventDefault()
#                     when 36 # begin
#                         @cursor.val = [ [ 0, 0, 1 ] ]
#                         e.preventDefault()
#                     when 37 # left (<-)
#                         npc = @cursor.val[ 0 ][ 0 ]
#                         npd = @cursor.val[ 0 ][ 1 ]
#                         if npc != npd and not e.shiftKey
#                             @cursor.val = [ [ Math.min( npc, npd ), Math.min( npc, npd ), 1 ] ]
#                         else if npc
#                             if e.ctrlKey
#                                 str = @props.obj.val
#                                 while npc and str[ npc - 1 ] in " \t\n"
#                                     npc -= 1
#                                 while npc and str[ npc - 1 ] not in " \t\n"
#                                     npc -= 1
#                             else
#                                 npc -= 1
#                             if e.shiftKey
#                                 @cursor.val = [ [ npc, npd, 1 ] ]
#                             else
#                                 @cursor.val = [ [ npc, npc, 1 ] ]
#                         e.preventDefault()
#                     when 38 # up
#                         @cursor.val = [ [ 0, 0, 1 ] ]
#                         e.preventDefault()
#                     when 39 # right (->)
#                         npc = @cursor.val[ 0 ][ 0 ]
#                         npd = @cursor.val[ 0 ][ 1 ]
#                         if npc != npd and not e.shiftKey
#                             @cursor.val = [ [ Math.max( npc, npd ), Math.max( npc, npd ), 1 ] ]
#                         else if npc < @props.obj.length
#                             if e.ctrlKey
#                                 str = @props.obj.val
#                                 while npc < str.length and str[ npc ] in " \t\n"
#                                     npc += 1
#                                 while npc < str.length and str[ npc ] not in " \t\n"
#                                     npc += 1
#                             else
#                                 npc += 1
#                             if e.shiftKey
#                                 @cursor.val = [ [ npc, npd, 1 ] ]
#                             else
#                                 @cursor.val = [ [ npc, npc, 1 ] ]
#                         e.preventDefault()
#                     when 40 # down
#                         @cursor.val = [ [ @props.obj.length, @props.obj.length, 1 ] ]
#                         e.preventDefault()
#                     when 46 # suppr
#                         npc = @cursor.val[ 0 ][ 0 ]
#                         npd = @cursor.val[ 0 ][ 1 ]
#                         if npc != npd
#                             beg = Math.min npc, npd
#                             end = Math.max npc, npd
#                             @props.obj.remove beg, end - beg
#                         else if npc < @props.obj.length
#                             if e.ctrlKey
#                                 str = @props.obj.val
#                                 while npc < str.length and str[ npc ] in " \t\n"
#                                     npc += 1
#                                 while npc < str.length and str[ npc ] not in " \t\n"
#                                     npc += 1
#                             else
#                                 npc += 1
#                             @props.obj.remove @cursor.val[ 0 ][ 0 ], npc - @cursor.val[ 0 ][ 0 ]
#                         e.preventDefault()
#                     # else
#                         # console.log e.keyCode
#
#             onKeyPress: ( e ) =>
#                 beg = Math.min @cursor.val[ 0 ][ 0 ], @cursor.val[ 0 ][ 1 ]
#                 end = Math.max @cursor.val[ 0 ][ 0 ], @cursor.val[ 0 ][ 1 ]
#                 if end != beg then @props.obj.remove beg, end - beg
#                 @props.obj.insert beg, String.NativeType.fromCharCode e.which
#                 e.preventDefault()
#
#             onMouseDown: ( e ) =>
#                 setTimeout ( => s = ReactInputSelection.getSelection @_input_element(); @cursor.val = [ [ s.start, s.end, 1 ] ] ), 0
#
#             onMouseUp: ( e ) =>
#                 s = ReactInputSelection.getSelection @_input_element()
#                 @cursor.val = [ [ s.start, s.end, 1 ] ]
#
#             onFocus: ( e ) =>
#                 s = ReactInputSelection.getSelection @_input_element()
#                 @cursor.val = [ [ s.start, s.end, 1 ] ]
#
#             onBlur: ( e ) =>
#                 @cursor.at( 0 ).at( 2 ).val = 0
#
#     _input_element: ->
#         ReactDOM.findDOMNode( this ).getElementsByTagName( 'input' )[ 0 ]
