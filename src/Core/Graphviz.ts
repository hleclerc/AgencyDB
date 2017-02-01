import methods            from "./methods";
import Rp                 from "./Rp"
import * as child_process from "child_process";
import * as fs            from "fs";

interface NodeInfo {
    style?: "dotted" | "dashed" | "plain";
    color?: string;
}

interface EdgeInfo {
    label   ?: string;
    style   ?: "dotted" | "dashed" | "plain";
    color   ?: string;
    subgraph?: NodeInfo;
}

function grepr( str: string ): string {
    return str.replace( /"/g, '\\"' );
}

export default
class Graphviz {
    static display( lst: Array<Rp>, base_filename = ".graph" ) {
        let gr = new Graphviz();
        for( const val of lst )
            methods[ "write_graphviz__b" ].call_1( val, gr );
        fs.writeFileSync( `${ base_filename }.dot`, `digraph gr {\n${ gr.text }}\n` );
        child_process.execSync( `dot -Tpdf '${ base_filename }.dot' > '${ base_filename }.pdf'`, { shell: "bash" } );
        child_process.execSync( `okular '${ base_filename }.pdf' 2> /dev/null` );
    }

    node( val: Rp, nb_inputs: number, nb_outputs: number, label: string, info?: NodeInfo ): boolean {
        if ( this.map_item.has( val ) )
            return false;

        this.info_item.set( val, { nb_inputs, nb_outputs } );
        const name = `node_${ this.map_item.size }`;
        this.map_item.set( val, name );
        this.text += `  ${ name }`;

        // attributes
        if ( this.use_ports ) {
            const m = this.use_ports >= 2 ? Math.max( nb_inputs, nb_outputs ) : nb_inputs;
            if ( m >= 0 ) {
                if ( m <= 1 ) {
                    label = `<f${ 0 }>` + label;
                } else {
                    for( let n = 0; n < m; ++n )
                        label += `|<f${ n }> `;
                }
            } 
        }
        const lst_attr = [ `label="${ grepr( label ) }"`, ...Object.keys( info || {} ).map( k => `${ k }="${ grepr( info[ k ] ) }"` ) ];
        if ( this.use_ports )
            lst_attr.push( "shape=record" );
        this.text += ` [${ lst_attr.join( ',' ) }]`;

        this.text += `;\n`;
        return true;
    }

    edge( src: Rp, num_src: number, dst: Rp, num_dst: number, info?: EdgeInfo ) {
        // src
        let name_src = this.map_item.get( src );
        if ( ! name_src ) { methods[ "write_graphviz__b" ].call_1( src, this ); name_src = this.map_item.get( src ); }

        if ( info && info.subgraph ) {
            this.text += `  subgraph cluster_${ this.map_item.size } {\n`;
            for( let k in info.subgraph )
                this.text += `  ${ k }=${ info.subgraph[ k ] };\n`;
        }

        // dst
        let name_dst = this.map_item.get( dst );
        if ( ! name_dst ) { methods[ "write_graphviz__b" ].call_1( dst, this ); name_dst = this.map_item.get( dst ); }

        if ( info && info.subgraph )
            this.text += `  }\n`;

        // edge line
        //this.text += `  ${ name_src }${ num_src >= 0 && false ? `:f${ num_src }` : '' } -> ${ name_dst }${ num_dst >= 0 && false ? `:f${ num_dst }` : '' }`;
        if ( this.use_ports == 2 ) {
            this.text += `  ${ name_src }${ num_src >= 0 ? `:f${ num_src }` : '' } -> ${ name_dst }${ num_dst >= 0 ? `:f${ num_dst }` : '' }`;
        } else if ( this.use_ports == 1 ) {
            this.text += `  ${ name_src }${ num_src >= 0 ? `:f${ num_src }` : '' } -> ${ name_dst }`;
            if ( this.info_item.get( dst ).nb_outputs > 1 )
                this.text += ` [label="${ num_dst }"]`;
        } else {
            this.text += `  ${ name_src } -> ${ name_dst }`;
            if ( this.info_item.get( src ).nb_inputs > 1 || this.info_item.get( dst ).nb_outputs > 1 )
                this.text += ` [label="${ this.info_item.get( src ).nb_inputs > 1 ? num_src : "" }->${ this.info_item.get( dst ).nb_outputs > 1 ? num_dst : "" }"]`;
        }

        if ( info ) {
            const lst_attr = Object.keys( info ).filter( k => k != "subgraph" ).map( k => `${ k }=${ grepr( info[ k ] ) }` );
            if ( lst_attr.length )
                this.text += ` [${ lst_attr.join( ',' ) }]`;
        }

        this.text += `;\n`;
    }

    map_item  = new Map<Rp,string>();
    info_item = new Map<Rp,{nb_inputs:number,nb_outputs:number}>();
    use_ports = 1;
    text      = "";
} 
