DevId = require "./DevId.js"

#
module.exports =
class UsrId
    constructor: ( dev = new DevId, num = 0 ) ->
        @dev = dev
        @num = num

    toString: ->
        "#{@dev}:#{@num}"
