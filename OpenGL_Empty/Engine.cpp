#include "Engine.h"
Engine::Engine()
{
}

Engine::~Engine()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	glfwTerminate();
}
namespace
{
	bool gameOver;
	unsigned int framecount;
	std::vector<int> removeObjs;
	GameObject * floorObj;
	Player * player;
	Camera * camptr;
	static std::map<int, bool> keyIsDown;
	static std::map<int, bool> keyWasDown;
	typedef void(*keyFunction)();
	std::map<int, keyFunction> keyDownFuncs;
	std::map<int, keyFunction> keyUpFuncs;
}
void mouseClick(GLFWwindow * windowPtr,
	int button, int action, int mods)
{
	keyIsDown[button] = action;
}
void keyCallback(GLFWwindow * window, int key,
	int scancode, int action, int mods)
{
	keyIsDown[key] = action;
	if (action == GLFW_PRESS && keyDownFuncs.count(key) != 0)
		(*keyDownFuncs[key])();
	else if (action == GLFW_RELEASE && keyUpFuncs.count(key) != 0)
		(*keyUpFuncs[key])();
	else
	{
		glm::mat3 rot = (glm::mat3)glm::yawPitchRoll(camptr->transform.rotation.y, camptr->transform.rotation.x, camptr->transform.rotation.z);
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_A)
				camptr->body.velocity += rot * glm::vec3(-1, 0, 0);
			if (key == GLFW_KEY_D)
				camptr->body.velocity += rot * glm::vec3(1, 0, 0);
			if (key == GLFW_KEY_W)
				camptr->body.velocity += rot * glm::vec3(0, 1, 0);
			if (key == GLFW_KEY_S)
				camptr->body.velocity += rot * glm::vec3(0, -1, 0);
			if (key == GLFW_KEY_Q)
				camptr->body.velocity += rot * glm::vec3(0, 0, -1);
			if (key == GLFW_KEY_E)
				camptr->body.velocity += rot * glm::vec3(0, 0, 1);
		}
		else if (action == GLFW_RELEASE)
		{
			camptr->body.velocity = { 0,0,0 };
		}


		float speed = 1.f;
		if (camptr->body.velocity != glm::vec3())
			camptr->body.velocity = glm::normalize(camptr->body.velocity) * speed;
	}
}
void leftPress()
{
	player->moveIn(-1);
}
void rightPress()
{
	player->moveIn(1);
}
void leftRelease()
{
	player->moveIn(0);
}
void rightRelease()
{
	player->moveIn(0);
}
void upKeyPress()
{
	player->jump();
}
void Engine::applyFriction(GameObject *obj, float dt)
{
	float sign = (std::abs(obj->body.velocity.x) / obj->body.velocity.x);
	if (obj->body.velocity.x != 0)
	{
		float force = sign * 0.02f;
		obj->body.force.x -= force;
	}
}
void Engine::calcVelocity(GameObject *obj, float dt)
{
	if (std::abs((obj->body.force.x  * dt) / (obj->body.mass)) > std::abs(obj->body.velocity.x) && obj->forceIn.x == 0)
		obj->body.velocity.x = 0;
	else
		obj->body.velocity.x += (obj->body.force.x  * dt) / (obj->body.mass);
	obj->body.velocity.y += (obj->body.force.y * dt) / (obj->body.mass);
	obj->body.velocity.z += (obj->body.force.z * dt) / (obj->body.mass);
}
void Engine::applyGravity(GameObject *obj)
{
	if (obj->transform.location.y >= -1.75 && !(obj->name == "images/wall.jpg"))
		obj->body.force.y -= 1;
	else
		obj->body.velocity.y = 0;
}

bool Engine::init()
{
	gameOver = false;
	keyDownFuncs[GLFW_KEY_LEFT] = &leftPress;
	keyDownFuncs[GLFW_KEY_RIGHT] = &rightPress;
	keyDownFuncs[GLFW_KEY_UP] = &upKeyPress;
	keyUpFuncs[GLFW_KEY_LEFT] = &leftRelease;
	keyUpFuncs[GLFW_KEY_RIGHT] = &leftRelease;

	imageIndex = 0;
	pos = -1.75;
	direction = 1;

	if (glfwInit() == GL_FALSE)
		return false;
	GLFWwindowPtr =
		glfwCreateWindow(800, 800, "Aaron Collins DSA1 Engine",
			NULL, NULL);
	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else {
		glfwTerminate();
		return false;
	}
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}
	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);
	imageNames.push_back("images/wall.jpg");
	imageNames.push_back("images/mario.png");
	imageNames.push_back("images/goomba.png");
	imageNames.push_back("images/bowser.png");
	imageNames.push_back("images/background.png");
	imageNames.push_back("images/dead.png");
	imageNames.push_back("images/block.png");
	imageNames.push_back("images/gameover.png");
	imageNames.push_back("images/coin.png");
	for (int i = 0; i < imageNames.size(); i++)
	{
		image = (FreeImage_Load(
			FreeImage_GetFileType(imageNames[i], 0),
			imageNames[i]));
		if (image == nullptr)
		{
			std::cout << "Failure!";
			return false;
		}
		image32bit = FreeImage_ConvertTo32Bits(image);
		FreeImage_Unload(image);
		glGenTextures(1, &textures[imageNames[i]]);
		glBindTexture(GL_TEXTURE_2D, textures[imageNames[i]]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_SRGB_ALPHA,
			FreeImage_GetWidth(image32bit),
			FreeImage_GetHeight(image32bit),
			0, GL_BGRA, GL_UNSIGNED_BYTE,
			(void*)FreeImage_GetBits(image32bit));
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		FreeImage_Unload(image32bit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	GameObject::Transform transform;
	locInds =
	{ 0, 1, 2,
		0, 2, 3 };
	vertCount = locInds.size();
	transform = {
		glm::vec3(-1, 1, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1),
		glm::mat4()
	};
	objects.push_back(
		new GameObject(
			genVertBufData(
	{
		{ { 2, 0, 0 },{ 1,1 } },
		{ { 0, 0, 0 },{ 0,1 } },
		{ { 0, -2, 0 },{ 0,0 } },
		{ { 2, -2, 0 },{ 1,0 } }
	}
				),
			&transform,
			imageNames[4],
			GameObject::Rigidbody{ { 0,0,0 },{ 0,0,0 },1 },
			0,
			{ 0, 0, 0 },
			GameObject::AxisAlignedBB,
			{ 2, 0.5, 0 }
	));
	transform = {
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1),
		glm::mat4() };
	objects.push_back(
		player = new Player(
			genVertBufData(
	{
		{ { 0.5, 0, 0 },{ 1,1 } },
		{ { 0, 0, 0 },{ 0,1 } },
		{ { 0, -0.5, 0 },{ 0,0 } },
		{ { 0.5, -0.5, 0 },{ 1,0 } }
	}
				),
			&transform,
			imageNames[1],
			GameObject::Rigidbody{ {0,0,0},{0,0,0},1 },
			0,
			{ 0, 0, 0 },
			GameObject::AxisAlignedBB,
			{ 0.5, 0.5, 0 }
	));
	transform = {
		glm::vec3(-1, -0.5, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1),
		glm::mat4()
	};
	objects.push_back(
		floorObj = new GameObject(
			genVertBufData(
	{
		{ { 2, 0, 0 },{ 0,1 } },
		{ { 0, 0, 0 },{ 1,1 } },
		{ { 0, -0.5, 0 },{ 1,0 } },
		{ { 2, -0.5, 0 },{ 0,0 } }
	}
				),
			&transform,
			imageNames[0],
			GameObject::Rigidbody{ { 0,0,0 },{ 0,0,0 },1 },
			0,
			{ 0, 0, 0 },
			GameObject::AxisAlignedBB,
			{ 2, 0.5, 0 }
	));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	camera = Camera();
	camptr = &camera;
	glfwSetCursorPos(GLFWwindowPtr, 800 * 0.5f, 800 * 0.5f);
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	return true;
}

bool Engine::bindImage(char* name)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[name]);
	glUniform1i(glGetUniformLocation(manager.getProgram(), "myTexture"), 0);
	return true;
}
std::vector<GameObject::Vertex> Engine::genVertBufData(std::vector<GameObject::Vertex> locs)
{
	std::vector<GameObject::Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = locs[locInds[i]];
	}
	return vertBufData;
}
void Engine::calcCamTransform()
{
	const float pi = 3.1415926535897f;
	float sens = .005;
	int w = 800, h = 800;
	double x, y;
	glfwGetCursorPos(GLFWwindowPtr, &x, &y);

	camera.transform.rotation.y -= sens * (x - w * 0.5f);
	camera.transform.rotation.x -= sens * (y - h * 0.5f);
	camera.transform.rotation.x =
		glm::clamp(camera.transform.rotation.x, -0.5f * pi, 0.5f * pi);
	glfwSetCursorPos(GLFWwindowPtr, w * 0.5f, h * 0.5f);
}
bool Engine::bufferModel()
{
	for (int i = 0; i < objects.size(); i++)
	{
		glGenVertexArrays(1, &objects[i]->vertArr);
		glGenBuffers(1, &vertBufs);
		glBindVertexArray(objects[i]->vertArr);

		glBindBuffer(GL_ARRAY_BUFFER, vertBufs);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GameObject::Vertex) * vertCount,
			&objects[i]->verBufData[0],
			GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GameObject::Vertex),
			0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GameObject::Vertex),
			(void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);
	}

	bindImage(imageNames[imageIndex]);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	return true;
}

bool Engine::gameLoop()
{
	timeCurrent = glfwGetTime();
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
	
		timePrevious = timeCurrent;
		timeCurrent = glfwGetTime();
		timeDelta = timeCurrent - timePrevious;
		spawnTime += timeDelta;
		coinTime += timeDelta;
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vertArr);
		framecount++;
		glm::mat4 identity = glm::mat4(1.0);
		calcCamTransform();
		camera.update(timeDelta);
		camera.calcCamMat();
		GLuint transform_loc = glGetUniformLocation(manager.getProgram(), "transform");
		GLuint cam_loc = glGetUniformLocation(manager.getProgram(), "camera");
		glUniformMatrix4fv(cam_loc, 1, GL_FALSE, &camera.cameraMat[0][0]);
		if (!gameOver)
		{
			keyWasDown = keyIsDown;
			glfwPollEvents();
			if (spawnTime > 1)
			{
				spawnEnemy("images/goomba.png");
				spawnTime = 0;
			}
			if (coinTime > 1.5)
			{
				spawnCoin();
				coinTime = 0;
			}
		}
		for (int x = 0; x < objects.size(); x++)
		{
			if (!gameOver)
			{
				objects[x]->applyPhysics(timeDelta, floorObj);
				objects[x]->move(timeDelta);

				if (objects[x] == player)
				{
					player->checkColls(floorObj);
				}
			}
			bindImage(objects[x]->name);
			objects[x]->transform.objWorldTransform = (
				glm::translate(objects[x]->transform.location) *
				glm::yawPitchRoll(objects[x]->transform.rotation.y, objects[x]->transform.rotation.x, objects[x]->transform.rotation.z) *
				glm::scale(objects[x]->transform.size));

			glUniformMatrix4fv(transform_loc, 1, GL_FALSE, &objects[x]->transform.objWorldTransform[0][0]);
			glBindVertexArray(objects[x]->vertArr);
			glDrawArrays(GL_TRIANGLES, 0, vertCount);
			objects[x]->body.force = { 0,0,0 };
			if (objects[x]->type == "Enemy")
			{
				if (objects[x]->collidesWith(player))
				{
					gameOver = true;
					GameOver();
				}
				if ((objects[x]->transform.location.y - objects[x]->dimensions.y) < -0.5)
					removeObjs.push_back(x);
			}
			else if (objects[x]->type == "Coin")
			{
				if (objects[x]->collidesWith(player))
				{
					removeObjs.push_back(x);
				}
			}
		}
		for (int i = 0; i < removeObjs.size(); i++)
		{
			objects.erase(objects.begin() + removeObjs[i]);
		}
		removeObjs.clear();
		glBindVertexArray(0);
		glfwSwapBuffers(GLFWwindowPtr);
	}
	glfwTerminate();
	return true;
}
void Engine::GameOver()
{
	GameObject::Transform transform = {
		glm::vec3(-0.5, 0.5, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1),
		glm::mat4() };
	objects.push_back(
		new GameObject(
			genVertBufData(
	{
		{ { 1, 0, 0 },{ 1,1 } },
		{ { 0, 0, 0 },{ 0,1 } },
		{ { 0, -1, 0 },{ 0,0 } },
		{ { 1, -1, 0 },{ 1,0 } }
	}
				),
			&transform,
			"images/gameover.png",
			GameObject::Rigidbody{ { 0,0,0 },{ 0,0,0 },1 },
			0,
			{ 0, 0, 0 },
			GameObject::AxisAlignedBB,
			{ 0.25, 0.25, 0 }
	));
	int i = objects.size() - 1;
	glGenVertexArrays(1, &objects[i]->vertArr);
	glGenBuffers(1, &vertBufs);
	glBindVertexArray(objects[i]->vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufs);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GameObject::Vertex) * vertCount,
		&objects[i]->verBufData[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GameObject::Vertex),
		0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GameObject::Vertex),
		(void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);
}
bool Engine::useShaders()
{
	if (manager.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl"))
	{
		glUseProgram(manager.getProgram());
		return true;
	}
	return false;
}
void Engine::spawnEnemy(char* name)
{
	int randx = rand() % 176 + (-100);
	float xpos = randx / 100.f;
	GameObject::Transform transform = {
		glm::vec3(xpos, 1, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1),
		glm::mat4() };
	objects.push_back(
		new Enemy(
			genVertBufData(
	{
		{ { 0.25, 0, 0 },{ 1,1 } },
		{ { 0, 0, 0 },{ 0,1 } },
		{ { 0, -0.25, 0 },{ 0,0 } },
		{ { 0.25, -0.25, 0 },{ 1,0 } }
	}
				),
			&transform,
			name,
			GameObject::Rigidbody{ { 0,0,0 },{ 0,0,0 },1 },
			0,
			{ 0, 0, 0 },
			GameObject::AxisAlignedBB,
			{ 0.25, 0.25, 0 }
	));
	int i = objects.size() - 1;
	glGenVertexArrays(1, &objects[i]->vertArr);
	glGenBuffers(1, &vertBufs);
	glBindVertexArray(objects[i]->vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufs);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GameObject::Vertex) * vertCount,
		&objects[i]->verBufData[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GameObject::Vertex),
		0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GameObject::Vertex),
		(void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);
}
void Engine::spawnCoin()
{
	int randx = rand() % 176 + (-100);
	float xpos = randx / 100.f;
	GameObject::Transform transform = {
		glm::vec3(xpos, -0.25, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1),
		glm::mat4() };
	objects.push_back(
		new GameObject(
			genVertBufData(
	{
		{ { 0.25, 0, 0 },{ 1,1 } },
		{ { 0, 0, 0 },{ 0,1 } },
		{ { 0, -0.25, 0 },{ 0,0 } },
		{ { 0.25, -0.25, 0 },{ 1,0 } }
	}
				),
			&transform,
			"images/coin.png",
			GameObject::Rigidbody{ { 0,0,0 },{ 0,0,0 },1 },
			0,
			{ 0, 0, 0 },
			GameObject::AxisAlignedBB,
			{ 0.25, 0.25, 0 }
	));
	int i = objects.size() - 1;
	glGenVertexArrays(1, &objects[i]->vertArr);
	glGenBuffers(1, &vertBufs);
	glBindVertexArray(objects[i]->vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufs);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GameObject::Vertex) * vertCount,
		&objects[i]->verBufData[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GameObject::Vertex),
		0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GameObject::Vertex),
		(void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);
}