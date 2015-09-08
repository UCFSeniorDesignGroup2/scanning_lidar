/*

ModelMesh<SimpleVertex>* Primitives::MakeSphere(float r, int vsects, int hsects)
{
    std::string name = "sphere";
    char buffer[50];
    sprintf(buffer, "%f%d%d",r,vsects,hsects);
    name += buffer;
    ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>(name);

    float thetastep = PI/(hsects+1);
    std::cout << thetastep << std::endl;
    float theta = 0;
    float phistep = 2*PI/vsects;
    float phi = 0;

    Float3f* zero = new Float3f(0,0,0);

    SimpleVertex* top = new SimpleVertex(new Float3f(0,0,r), new Float3f(0,0,r), zero);
    mesh->AddVertex(top);

    for(int i = 0; i < hsects; i++)
    {
        theta += thetastep;
        for(int j = 0; j < vsects; j++)
        {
            float x = cos(phi)*sin(theta) * r;
            float y = sin(phi)*sin(theta) * r;
            float z = cos(theta) * r;
            Float3f* f = new Float3f(x,y,z);
            SimpleVertex* v = new SimpleVertex(f,f,zero);
            mesh->AddVertex(v);
            phi += phistep;
        }
    }
    SimpleVertex* bottom = new SimpleVertex(new Float3f(0,0,-r), new Float3f(0,0,-r), zero);
    mesh->AddVertex(bottom);

    std::cout << "verts: " << mesh->GetVertices().size() << std::endl;



    //add top indices
    for(int i = 2; i < vsects+1; i++)
    {
        mesh->AddIndex(0);
        mesh->AddIndex(i-1);
        mesh->AddIndex(i);
    }
    mesh->AddIndex(0);
    mesh->AddIndex(1);
    mesh->AddIndex(vsects);



    for(int i = 0; i < hsects-1; i++)
    {
        for(int j = 0; j < vsects+1; j++)
        {
            mesh->AddIndex(vsects*i+j);
            mesh->AddIndex(vsects*i+j+1);
            mesh->AddIndex(vsects*(i+1)+j);
        }
        for(int j = 0; j < vsects+1; j++)
        {
            mesh->AddIndex(vsects*i+j+1);
            mesh->AddIndex(vsects*(i+1)+j+1);
            mesh->AddIndex(vsects*(i+1)+j);
        }
    }

    //add bottom indices
    for(int i = 1; i < vsects; i++)
    {
        mesh->AddIndex(mesh->GetVertices().size() - 1);
        mesh->AddIndex(mesh->GetVertices().size() - 1 - i);
        mesh->AddIndex(mesh->GetVertices().size() - 2 - i);
    }
    mesh->AddIndex(mesh->GetVertices().size() - 1);
    mesh->AddIndex(mesh->GetVertices().size() - 2);
    mesh->AddIndex(mesh->GetVertices().size() - vsects-1);



    physx::PxGeometry *g = new physx::PxSphereGeometry(r);
    mesh->SetBounds(g);

    return mesh;
}
*/

function Sphere(r, h, v, inv)
{
  
  var thetaStep = Math.PI / (h + 1);
  var theta = 0;
  var phistep = 2 * Math.PI / v;
  var phi = 0;
  
  var position = [];  
  var normal = [];
  var tex = [];  
  this.mIndex = [];

  position.push([0,0,r]);
  if(inv)
  {
    normal.push([0,0,-1]);
  }
  else
  {
    normal.push([0,0,1]);
  }
  tex.push([0,0,1]);
  
  for(var i = 0; i < h; i++)
  {
    theta += thetaStep;
    for(var j = 0; j < v; j++)
    {
      var x = Math.cos(phi) * Math.sin(theta) * r;
      var y = Math.sin(phi) * Math.sin(theta) * r;
      var z = Math.cos(theta) * r;
      var pos = [x,y,z];
      var norm = [0,0,0];
      vec3.normalize(norm, pos);
      if(inv)
      {
        vec3.scale(norm, norm, -1);
      }
      position.push(pos);
      normal.push(norm);
      tex.push(norm);
      phi += phistep; 
    }
  }

  position.push([0,0,-r]);
  if(inv)
  {
    normal.push([0,0,1])
  }
  else
  {
    normal.push([0,0,-1]);
  }
  tex.push([0,0,-1]);

  for(var i = 2; i < v+1; i++)
  {
    this.mIndex.push(0)
    this.mIndex.push(i-1);
    this.mIndex.push(i);
  }
  this.mIndex.push(0);
  this.mIndex.push(1);
  this.mIndex.push(v);
   

  for(var i = 0; i < h - 1; i++)
  {
    for(var j = 0; j < v + 1; j++)
    {
      this.mIndex.push(v * i + j);
      this.mIndex.push(v * i + j + 1);
      this.mIndex.push(v * (i+1) + j);
    }
    for(var j = 0; j < v + 1; j++)
    {
      this.mIndex.push(v * i + j + 1);
      this.mIndex.push(v * (i+1) + j + 1);
      this.mIndex.push(v * (i+1) + j);
    }
  }
  

  for(var i = 1; i < v; i++)
  {
    this.mIndex.push(position.length - 1);
    this.mIndex.push(position.length - 1 - i);
    this.mIndex.push(position.length - 2 - i);
  }

  this.mIndex.push(position.length - 1);
  this.mIndex.push(position.length - 2);
  this.mIndex.push(position.length - v - 1);

  this.mPosition = [];
  this.mNormal = [];
  this.mTex = [];

  for(var i = 0; i < position.length; i++)
  {
    for(var j = 0; j < 3; j++)
    {
      this.mPosition.push(position[i][j]);
      this.mNormal.push(normal[i][j]);
      this.mTex.push(tex[i][j]);

    }
  }

}
