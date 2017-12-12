MUI_Slider = require 'material-ui/lib/slider';
React      = require "react"
ReactDOM   = require "react-dom"

#
module.exports = React.createClass
  render: ->
      <MUI_Slider defaultValue={0.5} onChange={ ( e, val ) -> console.log e, val}/>

    #   <div className="commentBox">
    #     <h1>Comments</h1>
    #     <CommentList data={this.state.data} />
    #     <CommentForm onCommentSubmit={this.handleCommentSubmit} />
    #   </div>
