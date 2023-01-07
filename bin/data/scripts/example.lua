print("Hello from Lua!")

bang = false
magnitude = 0.0
kick = 0.0
snare = 0.0
hat = 0.0

local d = draw.new()
local col = color.new()
local mat = material.new()
local utils = utils.new()
local gl = gl.new()
local light = light.new()

function setup()
	print("Hello from Lua's setup function!")
	d:setCircleResolution(60)
	light:setup()
	light:setPosition(200, 100, 400)
	mat:setPBR(true)
	mat:setDiffuse(0.8, 0.2, 0.2)
	mat:setRoughness(0.5)
	mat:setShininess(0.2)
end

function update()

end

function draw()
	width = utils:getWidth()/2
	height = utils:getHeight()/2

	gl:enableDepthTest()
	light:enableLighting() --enableLighting in general
	light:enable() -- enable this specific light
	mat:beginMaterial()
	--if you don't push a matrix, you might end up rotating
	--and translating everything else, Theia draws afterwards,
	--like the code for example
	gl:pushMatrix()
	gl:translate(width, height, 0)
	gl:rotateX(-30)
	d:fill()
	d:drawSphere(0, 0, 0, 100-magnitude)
	gl:popMatrix()
	d:noFill()
	mat:endMaterial()
	d:drawCircle(width, height, 190+snare*10)
	light:disable()
	light:disableLighting()
	gl:disableDepthTest()
end

function exit()
	print("Goodbye from Lua!")
end
