import Caret                    from '../../Variable/String/Caret';
import String                   from '../../Variable/String';
import PatchId                  from '../../System/PatchId';
import DevId                    from '../../System/DevId';
import CaretCoordinates         from './CaretCoordinates';

import * as ReactDOM            from 'react-dom';
import * as React               from 'react';

declare function require( s: string );
var ReactInputSelection  = require( 'react/lib/ReactInputSelection' );

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

interface State {
    value: string;
}

interface Recc {
    div: HTMLElement;
    a  : HTMLElement;
    b  : HTMLElement;
    c  : HTMLElement;
    d  : HTMLElement;
    e  : HTMLElement;
    f  : HTMLElement;
    g  : HTMLElement;
    h  : HTMLElement;
}

interface PropsWithObj {
    obj: String;
}

//
export default class InputMixin<Props extends PropsWithObj> extends React.Component<Props,State> {
    // propTypes: {
    //     obj: function( props, propName, componentName ) {
    //         if ( ! ( props.obj instanceof String ) )
    //             return new Error( "Validation failed!" );
    //         return undefined;
    //     }
    // }
    _input_element() : HTMLTextAreaElement | HTMLInputElement {
        throw new Error( "_input_element has to be redefined in subclasses" );
    }

    componentWillUnmount() {
        this.props.obj.rm_onchange( this.onchange_id );
        this.props.obj.rem_cursor( this.cursor_id );
        if ( this.caret_coordinates )
            this.caret_coordinates.clean();
    }

    constructor( props ) {
        super( props );

        this.state = {
            value: this.props.obj.toString()
        };

        // change in obj => change in the displayed text + cursor position
        this.onchange_id = this.props.obj.onchange( () => {
            this.setState( { value: this.props.obj.val }, () => {
                // after react rendering (real DOM elements), we can access to the input element
                // in order to render the selection(s)
                let inp = this._input_element();
                let usd = [], usc = [];
                for( let cid of this.props.obj.get_cursor_ids() ) {
                    let c = this.props.obj.get_cursor( cid.loc_id, cid.dev_id )[ 0 ];
                    if ( cid.dev_id.loc() && cid.loc_id == this.cursor_id ) {
                        if ( c.foc )
                            ReactInputSelection.setSelection( inp, { start: c.min, end: c.max } );
                    } else if ( c.foc ) {
                        let lw = 1;

                        // prepare an object for the display
                        let recc = this.recc_map[ cid.dev_id.val + cid.loc_id ];
                        if ( ! recc ) {
                            recc = {} as Recc;

                            // container
                            recc.div                     = document.createElement( 'div' );
                            recc.div.style.position      = 'absolute';
                            recc.div.style.pointerEvents = 'none';
                            recc.div.style.top           = '0px';
                            recc.div.style.left          = '0px';

                            // lines
                            for( let i of "abcdefgh" ) {
                                let l                   = document.createElement( 'div' );
                                l.style.position        = 'absolute';
                                l.style.backgroundColor = 'red';
                                l.style.width           = lw + 'px';
                                l.style.height          = lw + 'px';

                                recc.div.appendChild( l );
                                recc[ i ] = l;
                            }
                            // recc.a.style.backgroundColor = 'green';
                            // recc.b.style.backgroundColor = 'blue';
                            // recc.c.style.backgroundColor = 'black';
                            // recc.d.style.backgroundColor = 'yellow';
                            // recc.e.style.backgroundColor = 'gray';
                            // recc.f.style.backgroundColor = 'pink';
                            // recc.g.style.backgroundColor = 'cyan';

                            document.body.appendChild( recc.div );
                            this.recc_map[ cid.dev_id.val + cid.loc_id ] = recc;
                        }

                        // say that we don't want to remove the display for this cursor
                        usc.push( cid.dev_id.val + cid.loc_id );

                        let ccp = this._caret_coord();
                        let pos = ccp.get_sel( c.min, c.max );
                        let fhe = 1.2 * pos.fontSize;

                        // let info = document.getElementById( 'info_0' );
                        // info.innerHTML = `
                        //     beg_left  : ${ pos.beg_left   }<br/>
                        //     beg_top   : ${ pos.beg_top    }<br/>
                        //     end_left  : ${ pos.end_left   }<br/>
                        //     end_top   : ${ pos.end_top    }<br/>
                        //     tot_width : ${ pos.tot_width  }<br/>
                        //     tot_left  : ${ pos.tot_left   }<br/>
                        //     tot_top   : ${ pos.tot_top    }<br/>
                        //     fontSize  : ${ pos.fontSize   }<br/>
                        //     lineHeight: ${ pos.lineHeight }<br/>
                        // `;

                        if ( c.beg == c.end ) {
                            recc.a.style.left       = pos.beg_left + 'px';
                            recc.a.style.top        = pos.beg_top  + 'px';
                            recc.a.style.height     = fhe          + 'px';

                            recc.b.style.visibility = 'hidden';
                            recc.c.style.visibility = 'hidden';
                            recc.d.style.visibility = 'hidden';
                            recc.e.style.visibility = 'hidden';
                            recc.f.style.visibility = 'hidden';
                            recc.g.style.visibility = 'hidden';
                            recc.h.style.visibility = 'hidden';

                        } else if ( pos.beg_top == pos.end_top || ( pos.end_top == pos.beg_top + pos.lineHeight && pos.end_left == pos.tot_left ) ) {
                            // -> a rectangle
                            let lp = pos.end_left == pos.tot_left ? pos.tot_left + pos.tot_width : pos.end_left;

                            recc.a.style.left       = pos.beg_left                      + 'px';
                            recc.a.style.top        = pos.beg_top                       + 'px';
                            recc.a.style.height     = fhe                               + 'px';

                            recc.b.style.left       = pos.beg_left                      + 'px';
                            recc.b.style.top        = pos.beg_top   + fhe               + 'px';
                            recc.b.style.width      = lp - pos.beg_left + 1             + 'px';
                            recc.b.style.visibility = 'visible';

                            recc.c.style.left       = lp                                + 'px';
                            recc.c.style.top        = pos.beg_top                       + 'px';
                            recc.c.style.height     = fhe                               + 'px';
                            recc.c.style.visibility = 'visible';

                            recc.d.style.left       = pos.beg_left                      + 'px';
                            recc.d.style.top        = pos.beg_top                       + 'px';
                            recc.d.style.width      = lp - pos.beg_left                 + 'px';
                            recc.d.style.visibility = 'visible';

                            recc.e.style.visibility = 'hidden';
                            recc.f.style.visibility = 'hidden';
                            recc.g.style.visibility = 'hidden';
                            recc.h.style.visibility = 'hidden';

                        } else {
                            let lw0 = pos.end_left == pos.tot_left ? pos.lineHeight : 0;
                            let aw = Math.round( 0.3 * pos.fontSize );
                            let lp = pos.tot_left + pos.tot_width;

                            /*
                                     c-- b --|
                                 |-f-|       e
                                 g       |---|
                                 a-------a
                            */
                            recc.a.style.left       = pos.end_left                           + 'px';
                            recc.a.style.top        = pos.end_top                            + 'px';
                            recc.a.style.height     = pos.lineHeight - lw0                   + 'px';

                            recc.b.style.left       = pos.beg_left                           + 'px';
                            recc.b.style.top        = pos.beg_top                            + 'px';
                            recc.b.style.width      = lp - pos.beg_left                      + 'px';
                            recc.b.style.visibility = 'visible';

                            recc.c.style.left       = pos.beg_left                           + 'px';
                            recc.c.style.top        = pos.beg_top                            + 'px';
                            recc.c.style.height     = pos.lineHeight - 2                     + 'px';
                            recc.c.style.visibility = 'visible';

                            recc.d.style.left       = pos.end_left                           + 'px';
                            recc.d.style.top        = pos.end_top                            + 'px';
                            recc.d.style.width      = lp - pos.end_left                      + 'px';
                            recc.d.style.visibility = 'visible';

                            recc.e.style.left       = lp                                     + 'px';
                            recc.e.style.top        = pos.beg_top                            + 'px';
                            recc.e.style.height     = pos.end_top - pos.beg_top              + 'px';
                            recc.e.style.visibility = 'visible';

                            recc.f.style.left       = pos.tot_left                           + 'px';
                            recc.f.style.top        = pos.beg_top + pos.lineHeight - 2       + 'px';
                            recc.f.style.width      = pos.beg_left - pos.tot_left            + 'px';
                            recc.f.style.visibility = 'visible';

                            recc.g.style.left       = pos.tot_left                           + 'px';
                            recc.g.style.top        = pos.beg_top + pos.lineHeight - 2       + 'px';
                            recc.g.style.height     = pos.end_top - pos.beg_top    + 2 - lw0 + 'px';
                            recc.g.style.visibility = 'visible';

                            recc.h.style.left       = pos.tot_left                           + 'px';
                            recc.h.style.top        = pos.end_top + pos.lineHeight - lw0     + 'px';
                            recc.h.style.width      = pos.end_left - pos.tot_left + lw       + 'px';
                            recc.h.style.visibility = 'visible';
                        }
                    }
                }

                // remove unused ext selection
                for( let n in this.recc_map ) {
                    if ( usc.indexOf( n ) < 0 ) {
                        this.recc_map[ n ].div.remove();
                        delete this.recc_map[ n ];
                    }
                }
            } );
        } );

        // attribute for input item
        this.aprops = {
            onChange: ( event ) => {
                // this.setState( {value: event.target.value} );
            },

            onKeyDown: ( e : KeyboardEvent ) => {
                switch ( e.keyCode ) {
                    case 8: { // backspace
                        let cur = (<Caret>this._get_cursor());
                        let npc = cur.beg;
                        let npd = cur.end;
                        if ( npc != npd ) {
                            let beg = Math.min( npc, npd );
                            let end = Math.max( npc, npd );
                            this.props.obj.remove( beg, end - beg );
                        } else if ( npc ) {
                            if ( e.ctrlKey ) {
                                let str = this.props.obj.val;
                                while ( npc && " \t\n".indexOf( str[ npc - 1 ] ) >= 0 )
                                    npc -= 1;
                                while ( npc && " \t\n".indexOf( str[ npc - 1 ] ) < 0 )
                                    npc -= 1;
                            } else {
                                npc -= 1;
                            }
                            this.props.obj.remove( npc, cur.beg - npc );
                        }
                        e.preventDefault();
                        break;
                    }
                    case 35: { // end
                        let l = this.props.obj.length.val;
                        this._set_cursor( l );
                        e.preventDefault();
                        break;
                    }
                    case 36: { // begin
                        this._set_cursor( 0 );
                        e.preventDefault();
                        break;
                    }
                    case 37: { // left (<-)
                        let cur = (<Caret>this._get_cursor());
                        let npc = cur.beg;
                        let npd = cur.end;
                        if ( npc != npd && ! e.shiftKey ) {
                            this._set_cursor( Math.min( npc, npd ) );
                        } else if ( npc ) {
                            if ( e.ctrlKey ) {
                                let str = this.props.obj.val;
                                while ( npc && " \t\n".indexOf( str[ npc - 1 ] ) >= 0 )
                                    npc -= 1;
                                while ( npc && " \t\n".indexOf( str[ npc - 1 ] ) < 0 )
                                    npc -= 1;
                            } else {
                                npc -= 1;
                            }

                            if ( e.shiftKey )
                                this._set_cursor( npc, npd );
                            else
                                this._set_cursor( npc );
                        }
                        e.preventDefault();
                        break;
                    }
                    case 38: { // up
                        let ccp = this._caret_coord();
                        let cur = this._get_cursor();
                        let pos = ccp.get_pos( cur.beg, false );
                        if ( ! this.pos_key_up_down )
                            this.pos_key_up_down = pos.left;
                        cur.beg = ccp.find_pos( this.pos_key_up_down, pos.top - 0.5 * pos.lineHeight );

                        if ( e.shiftKey )
                            this._set_cursor( cur.beg, cur.end );
                        else
                            this._set_cursor( cur.beg );

                        e.preventDefault();
                        break;
                    }
                    case 39: { // right (->)
                        let cur = <Caret>this._get_cursor();
                        let npc = cur.beg;
                        let npd = cur.end;
                        if ( npc != npd && ! e.shiftKey ) {
                            this._set_cursor( Math.max( npc, npd ) );
                        } else if ( npc < this.props.obj.length.val ) {
                            if ( e.ctrlKey ) {
                                let str = this.props.obj.val;
                                while ( npc < str.length && " \t\n".indexOf( str[ npc ] ) >= 0 )
                                    npc += 1;
                                while ( npc < str.length && " \t\n".indexOf( str[ npc ] ) < 0 )
                                    npc += 1;
                            } else {
                                npc += 1;
                            }

                            if ( e.shiftKey )
                                this._set_cursor( npc, npd );
                            else
                                this._set_cursor( npc, npc );
                        }
                        e.preventDefault();
                        break;
                    }
                    case 40: { // down
                        let ccp = this._caret_coord();
                        let cur = this._get_cursor();
                        let pos = ccp.get_pos( cur.max );
                        if ( ! this.pos_key_up_down )
                            this.pos_key_up_down = pos.left;
                        cur.beg = ccp.find_pos( this.pos_key_up_down, pos.top + 1.5 * pos.lineHeight );

                        if ( e.shiftKey )
                            this._set_cursor( cur.beg, cur.end );
                        else
                            this._set_cursor( cur.beg );

                        e.preventDefault();
                        break;
                    }
                    case 46: { // suppr
                        if ( ! e.shiftKey ) {
                            let cur = (<Caret>this._get_cursor());
                            let npc = cur.beg;
                            let npd = cur.end;
                            if ( npc != npd ) {
                                let beg = Math.min( npc, npd );
                                let end = Math.max( npc, npd );
                                this.props.obj.remove( beg, end - beg );
                            } else if ( npc < this.props.obj.length.val ) {
                                if ( e.ctrlKey ) {
                                    let str = this.props.obj.val
                                    while ( npc < str.length && " \t\n".indexOf( str[ npc ] ) >= 0 )
                                        npc += 1;
                                    while ( npc < str.length && " \t\n".indexOf( str[ npc ] ) < 0 )
                                        npc += 1;
                                } else{
                                    npc += 1;
                                }

                                this.props.obj.remove( cur.beg, npc - cur.beg );
                            }
                            e.preventDefault();
                        }
                        break;
                    }
                    case 65: { // A
                        if ( e.ctrlKey ) {
                            this._set_cursor( 0, this.props.obj.length.val );
                            e.preventDefault();
                        }
                        break;
                    }
                    case 90: { // Z
                        if ( e.ctrlKey ) {
                            throw new Error( "TODO: undo" );
                        }
                        break;
                    }
                }

                if ( e.keyCode != 38 && e.keyCode != 40 && e.keyCode != 27 && e.keyCode != 17 && e.keyCode != 16 && e.keyCode != 225 && e.keyCode != 91 && e.keyCode != 20 )
                    delete this.pos_key_up_down;

            },

            onKeyPress: ( e : KeyboardEvent ) => {
                if ( ! e.ctrlKey ) {
                    let cur = (<Caret>this._get_cursor());
                    let beg = cur.min;
                    let end = cur.max;
                    if ( end != beg ) this.props.obj.remove( beg, end - beg );
                    this.props.obj.insert( beg, String.NativeType.fromCharCode( e.which ) );
                    e.preventDefault();
                }
            },

            onCopy: ( e: ClipboardEvent ) => {
                let cur = (<Caret>this._get_cursor());
                e.clipboardData.setData( 'Text', <string>this.props.obj.val.slice( cur.min, cur.max ) );
                e.preventDefault();
            },

            onCut: ( e: ClipboardEvent ) => {
                let cur = (<Caret>this._get_cursor());
                e.clipboardData.setData( 'Text', <string>this.props.obj.val.slice( cur.min, cur.max ) );
                this.props.obj.remove( cur.min, cur.max - cur.min );
                e.preventDefault();
            },

            onPaste: ( e: ClipboardEvent ) => {
                let cur = (<Caret>this._get_cursor());
                if ( cur.max != cur.min )
                    this.props.obj.remove( cur.min, cur.max - cur.min );
                this.props.obj.insert( cur.min, e.clipboardData.getData( 'Text' ) );
                e.preventDefault();
            },

            // onDrag onDragEnd onDragEnter onDragExit
            // onDragLeave onDragOver onDragStart
            onDragStart: ( e: DragEvent ) => {
                let cur = (<Caret>this._get_cursor());
                e.dataTransfer.setData( 'Text', this.props.obj.val.slice( cur.min, cur.max ) );
            },

            onDrop: ( e: DragEvent ) => {
                let ccp = this._caret_coord();
                let ins = ccp.find_pos( e.clientX, e.clientY );
                this.props.obj.insert( ins, e.dataTransfer.getData( 'Text' ) );
                e.preventDefault();
            },

            onMouseDown: ( e : MouseEvent ) => {
                let ccp = this._caret_coord();
                let off = ccp.find_pos( e.clientX, e.clientY );
                let cur = this._get_cursor();
                //
                if ( cur.foc == 0 || off < cur.min || off >= cur.max ) {
                    this.mouse_down = true;
                    this._set_cursor( off );
                    this._input_element().focus();
                    e.preventDefault();
                }
            },

            onDoubleClick: ( e : MouseEvent ) => {
                let ccp = this._caret_coord();
                let beg = ccp.find_pos( e.clientX, e.clientY );
                let str = this.props.obj.val as string;
                let end = beg;
                while ( beg - 1 >= 0 && " \t\n\r".indexOf( str[ beg - 1 ] ) < 0 )
                    --beg;
                while ( end < str.length && " \t\n\r".indexOf( str[ end ] ) < 0 )
                    ++end;
                this._set_cursor( beg, end );
                this._input_element().focus();
                e.preventDefault();
            },

            onMouseMove: ( e ) => {
                if ( this.mouse_down ) {
                    let cur = this._get_cursor();
                    let ccp = this._caret_coord();
                    this._set_cursor( cur.beg, ccp.find_pos( e.clientX, e.clientY ) );
                }
            },

            onMouseUp: ( e ) => {
                this.mouse_down = false;
            },

            onFocus: ( e : FocusEvent ) => {
                setTimeout( () => {
                    let cur = (<Caret>this._get_cursor());
                    if ( ! cur.foc ) {
                        this._set_cursor( cur.beg, cur.end, 1 );
                    }
                }, 0 );
            },

            onBlur: ( e : FocusEvent ) => {
                this.mouse_down = false;
                // we need a timeout (at least for firefox to know the reason of the blur: focus to another element ? window is blured ?)
                setTimeout( () => {
                    let cur = <Caret>this._get_cursor();
                    if ( document.hasFocus() )
                        this._set_cursor( 0, this.props.obj.length.val, 0 );
                    else
                        this._set_cursor( cur.beg, cur.end, 0 );
                }, 0 );
            }
        };

        // make a new (inactive) cursor
        // this.cursor_id  = this.props.obj.new_cursor( [ Caret.New( 0, this.props.obj.length.val, 0 ) ] );
        this.mouse_down = false;
        this.recc_map   = {}; // divs for complex selections (several lines)
    }

    _caret_coord() : CaretCoordinates {
        if ( ! this.caret_coordinates )
            this.caret_coordinates = new CaretCoordinates( this._input_element() );
        return this.caret_coordinates;
    }

    _get_cursor() : Caret {
        if ( this.cursor_id ) {
            let cur = this.props.obj.get_cursor( this.cursor_id );
            if ( cur )
                return cur.length ? cur[ 0 ] : Caret.New( 0, this.props.obj.length.val, 0 );
        }
        let res = Caret.New( 0, this.props.obj.length.val, 0 );
        this.cursor_id = this.props.obj.new_cursor( [ res ] );
        return res;
    }

    _set_cursor( beg : number, end = beg, foc = 1 ) {
        return this.props.obj.set_cursor( this.cursor_id, [ Caret.New( beg, end, foc ) ] );
    }

    _input_props() {
        return (<any>Object).assign( {}, this.aprops, this.props, { value: this.state.value } );
    }

    caret_coordinates: CaretCoordinates;
    onchange_id      : PatchId;
    mouse_down       : boolean;
    cursor_id        : number;
    pos_key_up_down  : number;
    recc_map         : { [key: string]: Recc };
    aprops           : {};
}
