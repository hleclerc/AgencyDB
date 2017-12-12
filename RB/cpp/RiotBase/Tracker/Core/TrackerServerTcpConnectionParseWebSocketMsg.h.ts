import TrackerServerCmds from '../../../../common/TrackerServerCmds';
import Handlebars        from '../../../../common/Hb';

console.log( Handlebars.compile( `
{{#each cmds}}
case {{@index}}: {
    DISP_DEBUG( "recv {{#trim}}{{name}}{{/trim}} from {}", session->dev );
    {{#each args}}
    {{cpp_decl_cb}}{{name}} = bs.read();
    {{/each}}
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "{{#trim}}{{name}}{{/trim}}" ) )
        return false;
    recv_{{#trim}}{{name}}{{/trim}}({{#join_spba args}}std::move( {{name}} ){{/join_spba}});
    break;
}
{{/each}}
` )( { cmds : TrackerServerCmds } ) );
