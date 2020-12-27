

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;
layout (location = 4) in int do_str;

out vec3 Position;   // transform feedback
out vec3 Velocity;   // transform feedback
out float StartTime; // transform feedback
out float Transp;    // fragment shader

uniform float Time;  
uniform float H;     // time between frames
uniform vec3 Accel;  // particle acceleration
uniform float ParticleLifetime;  
uniform float MinParticleSize;
uniform float MaxParticleSize;

uniform mat4 MVP;


void update() {
    // update position & velocity 
    Position = VertexPosition;
    Velocity = VertexVelocity;
    StartTime = VertexStartTime;

    if (Time >= StartTime) {

        float age = Time - StartTime;

        if (age > ParticleLifetime) {

            // particle is past it's lifetime, recycle
            Position = vec3(0.0);
            Velocity = VertexInitialVelocity;
            StartTime = Time;
        } else {

            // particle is alive, update
            Position += Velocity * H;
            Velocity += Accel * H;
        }
    }
}


void render() {
    float age = Time - VertexStartTime;
    Transp = 0.0;
    if( Time >= VertexStartTime ) {
        float agePct = age/ParticleLifetime;
        Transp = 1.0 - agePct;
        gl_PointSize = mix(MinParticleSize,MaxParticleSize,agePct);
    }
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}



void main()
{
// call render() or update()
	if(do_str==0)
		render();
	else
    
		update();
}