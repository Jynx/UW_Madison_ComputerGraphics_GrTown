#version 330
 
out vec4 outputF;
uniform bool day;
 
void main()
{
	if(day){
        outputF = vec4(.529, 0.808, 0.980, 1.0);
	}
    else {
		outputF = vec4(1.0,  0.843, 0.0, 1.0);
	}
} 