import TrackerClientCmds from '../../../../common/TrackerClientCmds';
import Handlebars        from '../../../../common/Hb';

console.log( Handlebars.compile( `
{{#each cmds}}
void TrackerServerTcpConnection::send_{{#trim}}{{name}}{{/trim}}({{#join_spba args}}{{&cpp_type_cb}}{{name}}{{/join_spba}}) {
    DISP_DEBUG( "send {{#trim}}{{name}}{{/trim}} to {}", session->dev );
    bs_out_auto_close() << {{ @index }}u{{#each args}} << {{name}}{{/each}};
}
{{/each}}
` )( { cmds : TrackerClientCmds } ) );
