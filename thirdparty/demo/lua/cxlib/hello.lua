-- save the used globals
local math, require, setmetatable, pcall =
      math, require, setmetatable, pcall

-- create the Event module
--local uxwindow = require "lua_cxlib"
--local uhelpwindow = require "lua_cxlib"

require "lua_cxlib"

lua_cxlib.uhelpwindow()
lua_cxlib.uxwindow()

