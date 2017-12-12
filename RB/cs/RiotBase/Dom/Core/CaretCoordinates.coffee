# code stolen from https://github.com/component/textarea-caret-position

# The properties that we copy into a mirrored div.
# Note that some browsers, such as Firefox,
# do not concatenate properties, i.e. padding-top, bottom etc. -> padding,
# so we have to do every single property specifically.
properties = [
  'direction',  # RTL support
  'boxSizing',
  'width',  # on Chrome and IE, exclude the scrollbar, so the mirror div wraps exactly as the textarea does
  'height',
  'overflowX',
  'overflowY',  # copy the scrollbar for IE

  'borderTopWidth',
  'borderRightWidth',
  'borderBottomWidth',
  'borderLeftWidth',

  'paddingTop',
  'paddingRight',
  'paddingBottom',
  'paddingLeft',

  # https://developer.mozilla.org/en-US/docs/Web/CSS/font
  'fontStyle',
  'fontVariant',
  'fontWeight',
  'fontStretch',
  'fontSize',
  'fontSizeAdjust',
  'lineHeight',
  'fontFamily',

  'textAlign',
  'textTransform',
  'textIndent',
  'textDecoration',  # might not make a difference, but better be safe

  'letterSpacing',
  'wordSpacing'
]

get_left = ( l ) -> if l? then l.offsetLeft - l.scrollLeft + l.clientLeft + get_left( l.offsetParent ) else 0
get_top  = ( l ) -> if l? then l.offsetTop  - l.scrollTop  + l.clientTop  + get_top(  l.offsetParent ) else 0

module.exports =
class CaretCoordinates
    constructor: ( element ) ->
        @element = element
        @computed = if getComputedStyle? then getComputedStyle( element ) else element.currentStyle # currentStyle for IE < 9

        # create a div element
        @div = document.createElement 'div'
        document.body.appendChild @div

        # default textarea styles
        @div.style.whiteSpace = 'pre-wrap'
        if element.nodeName != 'INPUT'
            @div.style.wordWrap = 'break-word' # only for textarea-s

        # position off-screen
        @div.style.position = 'fixed'
        @div.style.left = "0px"
        @div.style.top = "0px"
        @div.style.visibility = 'hidden' # not 'display: none' because we want rendering

        # transfer the element's properties to the div
        properties.forEach ( prop ) =>
            @div.style[ prop ] = @computed[ prop ]

        @div.style.overflow = 'hidden' # for Chrome to not render a scrollbar; IE keeps overflowY = 'scroll'

        @divText = document.createTextNode ''
        @span = document.createElement 'span'
        @spanText = document.createTextNode ''
        @div.appendChild @divText
        @span.appendChild @spanText
        @div.appendChild @span

    get: ( index ) ->
        # the second special handling for input type="text" vs textarea: spaces need to be replaced with non-breaking spaces - http://stackoverflow.com/a/13402035/1269037
        if @element.nodeName == 'INPUT'
            @divText.nodeValue = @element.value.substring( 0, index ).replace /\s/g, "\u00a0"
        else
            @divText.nodeValue = @element.value.substring( 0, index )

        # Wrapping must be replicated *exactly*, including when a long word gets
        # onto the next line, with whitespace at the end of the line before (#7).
        # The  *only* reliable way to do that is to copy the *entire* rest of the
        # textarea's content into the <span> created at the caret position.
        # for inputs, just '.' would be enough, but why bother?
        @spanText.nodeValue = @element.value.substring( index ) or '.'  # || because a completely empty faux span doesn't render at all

        # if line-height != height
        atop = 0
        if @element.nodeName == 'INPUT'
            le = if @computed[ 'line-height' ].slice( -2 ) == "px" then parseInt( @computed[ 'line-height' ], 10 ) else 1.1 *  parseInt( @computed[ 'font-size' ], 10 )
            he = parseInt @computed[ 'height' ], 10
            atop = ( he - le ) / 2;

        top     : get_top(  @span ) + parseInt( @computed[ 'borderTopWidth'  ], 10 ) - 2 * parseInt( @computed[ 'padding-top'  ], 10 ) + atop
        left    : get_left( @span ) + parseInt( @computed[ 'borderLeftWidth' ], 10 ) - 2 * parseInt( @computed[ 'padding-left' ], 10 )
        fontSize: parseInt( @computed[ 'font-size' ], 10 )

    clean: ->
        @div.remove()

#
# # real position of an object
# get_left = ( l ) -> if l? then l.offsetLeft - l.scrollLeft + l.clientLeft + get_left( l.offsetParent ) else 0
# get_top  = ( l ) -> if l? then l.offsetTop  - l.scrollTop  + l.clientTop  + get_top(  l.offsetParent ) else 0
#
# # code stolen from https://github.com/component/textarea-caret-position
#
# # The properties that we copy into a mirrored div.
# # Note that some browsers, such as Firefox,
# # do not concatenate properties, i.e. padding-top, bottom etc. -> padding,
# # so we have to do every single property specifically.
# properties = [
#   'direction',  # RTL support
#   'boxSizing',
#   'width',  # on Chrome and IE, exclude the scrollbar, so the mirror div wraps exactly as the textarea does
#   'height',
#   'overflowX',
#   'overflowY',  # copy the scrollbar for IE
#
#   'borderTopWidth',
#   'borderRightWidth',
#   'borderBottomWidth',
#   'borderLeftWidth',
#
#   'paddingTop',
#   'paddingRight',
#   'paddingBottom',
#   'paddingLeft',
#
#   # https://developer.mozilla.org/en-US/docs/Web/CSS/font
#   'fontStyle',
#   'fontVariant',
#   'fontWeight',
#   'fontStretch',
#   'fontSize',
#   'fontSizeAdjust',
#   'lineHeight',
#   'fontFamily',
#
#   'textAlign',
#   'textTransform',
#   'textIndent',
#   'textDecoration',  # might not make a difference, but better be safe
#
#   'letterSpacing',
#   'wordSpacing'
# ]
#
# module.exports =
# class CaretCoordinates
#     constructor: ( element ) ->
#         @element = element
#         @div = document.createElement 'div'
#         element.parentNode.insertBefore @div, element
#
#         style = @div.style
#         @computed = if getComputedStyle? then getComputedStyle( element ) else element.currentStyle # currentStyle for IE < 9
#
#         # default textarea styles
#         style.whiteSpace = 'pre-wrap'
#         if element.nodeName != 'INPUT'
#             style.wordWrap = 'break-word' # only for textarea-s
#
#         # position off-screen
#         style.position = 'absolute' # required to return coordinates properly
#         style.visibility = 'hidden' # not 'display: none' because we want rendering
#
#         # transfer the element's properties to the div
#         properties.forEach ( prop ) =>
#           style[ prop ] = @computed[ prop ]
#
#         style.overflow = 'hidden' # for Chrome to not render a scrollbar; IE keeps overflowY = 'scroll'
#
#         @divText = document.createTextNode ''
#         @span = document.createElement 'span'
#         @spanText = document.createTextNode ''
#         @div.appendChild @divText
#         @span.appendChild @spanText
#         @div.appendChild @span
#
#     get: ( index ) ->
#         # calculate left offset
#         @divText.nodeValue = @element.value.substring 0, index
#
#         # the second special handling for input type="text" vs textarea: spaces need to be replaced with non-breaking spaces - http://stackoverflow.com/a/13402035/1269037
#         if @element.nodeName == 'INPUT'
#             @divText.nodeValue = @divText.nodeValue.replace /\s/g, "\u00a0"
#
#         # Wrapping must be replicated *exactly*, including when a long word gets
#         # onto the next line, with whitespace at the end of the line before (#7).
#         # The  *only* reliable way to do that is to copy the *entire* rest of the
#         # textarea's content into the <span> created at the caret position.
#         # for inputs, just '.' would be enough, but why bother?
#         @spanText.nodeValue = @element.value.substring( index ) or '.'  # || because a completely empty faux span doesn't render at all
#
#         atop = if @element.nodeName == 'INPUT'
#             le = if @computed[ 'line-height' ].slice( -2 ) == "px" then parseInt( @computed[ 'line-height' ], 10 ) else 1.1 *  parseInt( @computed[ 'font-size' ], 10 )
#             he = parseInt @computed[ 'height' ], 10
#             ( he - le ) / 2;
#         else
#             0
#
#         top     : get_top(  @span ) - get_top(  @element.parentNode ) + parseInt( @computed[ 'borderTopWidth'  ], 10 ) + atop
#         left    : get_left( @span ) - get_left( @element.parentNode ) + parseInt( @computed[ 'borderLeftWidth' ], 10 )
#         fontSize: parseInt( @computed[ 'font-size' ], 10 )
