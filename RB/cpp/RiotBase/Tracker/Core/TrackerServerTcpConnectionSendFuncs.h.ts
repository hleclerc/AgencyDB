import TrackerClientCmds from '../../../../common/TrackerClientCmds';
import Handlebars        from '../../../../common/Hb';

console.log( Handlebars.compile( `
{{#each cmds}}
void send_{{name}}({{#join_spba args}}{{&cpp_type_cb}}{{name}}{{/join_spba}});
{{/each}}
` )( { cmds : TrackerClientCmds } ) );
