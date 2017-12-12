// code stolen from https://github.com/component/textarea-caret-position

// The properties that we copy into a mirrored div.
// Note that some browsers, such as Firefox,
// do not concatenate properties, i.e. padding-top, bottom etc. -> padding,
// so we have to do every single property specifically.
let properties = [
  'direction',  // RTL support
  'boxSizing',
  'width',  // on Chrome and IE, exclude the scrollbar, so the mirror div wraps exactly as the textarea does
  'height',
  'overflowX',
  'overflowY',  // copy the scrollbar for IE

  'borderTopWidth',
  'borderRightWidth',
  'borderBottomWidth',
  'borderLeftWidth',

  'borderTopStyle',
  'borderRightStyle',
  'borderBottomStyle',
  'borderLeftStyle',

  'paddingTop',
  'paddingRight',
  'paddingBottom',
  'paddingLeft',

  // https://developer.mozilla.org/en-US/docs/Web/CSS/font
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
  'textDecoration',  // might not make a difference, but better be safe

  'letterSpacing',
  'wordSpacing'
];

function get_left( l ) { return l ? l.offsetLeft - l.scrollLeft + l.clientLeft + get_left( l.offsetParent ) : 0; }
function get_top ( l ) { return l ? l.offsetTop  - l.scrollTop  + l.clientTop  + get_top(  l.offsetParent ) : 0; }

interface CoordsPos {
    top       : number; // left
    left      : number; // top
    tot_width : number;
    tot_left  : number;
    tot_top   : number;
    fontSize  : number;
    lineHeight: number;
}

interface CoordsSel {
    beg_left  : number; // left
    beg_top   : number; // top
    end_left  : number;
    end_top   : number;
    tot_width : number;
    tot_left  : number;
    tot_top   : number;
    fontSize  : number;
    lineHeight: number;
}

export default
class CaretCoordinates {
    element : HTMLTextAreaElement | HTMLInputElement; // textarea or input element
    computed: any;                                    // css computed stuff from element
    div     : HTMLElement;                            // not displayed clone of element
    mid_elem: HTMLElement;
    end_elem: HTMLElement;
    beg_text: Text;
    mid_text: Text;
    end_text: Text;

    constructor( element : HTMLTextAreaElement | HTMLInputElement ) {
        this.element = element;
        this.computed = getComputedStyle ? getComputedStyle( element ) : ( element as any ).currentStyle; // currentStyle for IE < 9

        // create a div element
        this.div = document.createElement( 'div' );
        document.body.appendChild( this.div );

        // default textarea styles
        this.div.style.whiteSpace = 'pre-wrap';
        if ( element.nodeName != 'INPUT' )
            this.div.style.wordWrap = 'break-word'; // only for textarea-s

        // position off-screen
        this.div.style.position = 'fixed';
        this.div.style.visibility = 'hidden'; // not 'display: none' because we want rendering

        // transfer the element's properties to the div
        properties.forEach( ( prop ) =>
            this.div.style[ prop ] = this.computed[ prop ]
        );

        this.div.style.overflow = 'hidden'; // for Chrome to not render a scrollbar; IE keeps overflowY = 'scroll'

        // beg
        this.beg_text = document.createTextNode( '' );
        this.div.appendChild( this.beg_text );

        // mid
        this.mid_elem = document.createElement( 'span' );
        this.mid_text = document.createTextNode( '' );
        this.mid_elem.appendChild( this.mid_text );
        this.div.appendChild( this.mid_elem );
        this.mid_elem.style.color = 'red';

        // end
        this.end_elem = document.createElement( 'span' );
        this.end_text = document.createTextNode( '' );
        this.end_elem.appendChild( this.end_text );
        this.div.appendChild( this.end_elem );
        this.end_elem.style.color = 'green';
    }

    str() : string {
        return this.element.value;
    }

    get_pos( ind : number, boundary_corrections = true ) : CoordsPos {
        let str = this.str();

        // if line-height != height
        let fnts = parseInt( this.computed[ 'font-size' ], 10 );
        let lihe = this.computed[ 'line-height' ].slice( -2 ) == "px" ? parseInt( this.computed[ 'line-height' ], 10 ) : Math.ceil( 1.3 * fnts );

        // position of the div. Do we need to test if there is a change ?
        let rect = this.element.getBoundingClientRect();
        this.div.style.left = rect.left + "px";
        this.div.style.top = rect.top + "px";

        // the second special handling for input type="text" vs textarea: spaces need to be replaced with non-breaking spaces - http://stackoverflow.com/a/13402035/1269037
        this.beg_text.nodeValue =   this.element.nodeName == 'INPUT' ? str.slice(   0, ind ).replace( /\s/g, "\u00a0" ) : str.slice(   0, ind );
        this.mid_text.nodeValue = ( this.element.nodeName == 'INPUT' ? str.slice( ind      ).replace( /\s/g, "\u00a0" ) : str.slice( ind      ) ) || ".";
        this.end_text.nodeValue = "";

        return {
            left      : get_left( this.mid_elem ),
            top       : get_top ( this.mid_elem ),
            tot_width : rect.width,
            tot_left  : rect.left,
            tot_top   : rect.top,
            fontSize  : fnts,
            lineHeight: lihe
        };
    }

    get_sel( ind_0 : number, ind_1 = ind_0, boundary_corrections = true ) : CoordsSel {
        let str = this.str();
        if ( ind_0 == ind_1 ) {
            let pos = this.get_pos( ind_0, boundary_corrections );
            return {
                beg_left  : pos.left,
                beg_top   : pos.top ,
                end_left  : pos.left,
                end_top   : pos.top,
                tot_width : pos.tot_width,
                tot_left  : pos.tot_left,
                tot_top   : pos.tot_top,
                fontSize  : pos.fontSize,
                lineHeight: pos.lineHeight
            };
        }

        // if line-height != height
        let fnts = parseInt( this.computed[ 'font-size' ], 10 );
        let lihe = this.computed[ 'line-height' ].slice( -2 ) == "px" ? parseInt( this.computed[ 'line-height' ], 10 ) : Math.ceil( 1.5 * fnts );

        // position of the div. Do we need to test if there is a change ?
        let rect = this.element.getBoundingClientRect();
        this.div.style.left = rect.left + "px";
        this.div.style.top  = rect.top  + "px";

        // the second special handling for input type="text" vs textarea: spaces need to be replaced with non-breaking spaces - http://stackoverflow.com/a/13402035/1269037
        this.beg_text.nodeValue =   this.element.nodeName == 'INPUT' ? str.slice(     0, ind_0 ).replace( /\s/g, "\u00a0" ) : str.slice(     0, ind_0 );
        this.end_text.nodeValue = ( this.element.nodeName == 'INPUT' ? str.slice( ind_1        ).replace( /\s/g, "\u00a0" ) : str.slice( ind_1        ) ) || ".";  // because a completely empty faux span doesn't render at all
        this.mid_text.nodeValue =   this.element.nodeName == 'INPUT' ? str.slice( ind_0, ind_1 ).replace( /\s/g, "\u00a0" ) : str.slice( ind_0, ind_1 );

        return {
            beg_left  : get_left( this.mid_elem ),
            beg_top   : get_top ( this.mid_elem ),
            end_left  : get_left( this.end_elem ),
            end_top   : get_top ( this.end_elem ),
            tot_width : rect.width,
            tot_left  : rect.left,
            tot_top   : rect.top,
            fontSize  : fnts,
            lineHeight: lihe
        };
    }

    clean() {
        this.div.remove()
    }

    find_pos( x : number, y : number ) : number {
        let str = this.str(), beg = 0, end = str.length;
        let pbeg = null as CoordsPos, pend = null as CoordsPos;
        while ( beg < end - 1 ) {
            let mid = Math.floor( ( beg + end ) / 2 );
            let car = this.get_pos( mid, false );
            if ( car.top + car.lineHeight < y || ( car.top < y && car.left < x ) ) {
                pbeg = car;
                pend = null;
                beg  = mid;
            } else {
                pbeg = null;
                pend = car;
                end  = mid;
            }
        }
        if ( ! pbeg ) pbeg = this.get_pos( beg, false );
        if ( ! pend ) pend = this.get_pos( end, false );
        return y < pend.top + pend.lineHeight && ( y < pend.top || Math.abs( x - pbeg.left ) < Math.abs( x - pend.left ) ) ? beg : end;
    }

    get elem_width() { return this.element.clientWidth; }
    get elem_left () { return get_left( this.element ); }
    get elem_top  () { return get_top ( this.element ); }
}
