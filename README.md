# OGLEngine
Engine that was created on openGL
как добавлять свои фигуры и материалы


при добавлении объекта:
необходимо написать:
 функцию определения расстояния 
цикл в котором перебираются все объекты как этот
например:


	bool getSphClr(in vec3 me,in vec3 lookTo,in ivec4 in1, in ivec4 in2, out float len,out int id)
	{
		vec4 sphs1 ;
		float distan1,distan2,lenNow = inf;
		len = lenNow;
		in1/=4;
		in2/=4;
		if(in1.x==0) return false;
		int id_T=0;

		for (int i = in2.x;i<in2.y; i+=in1.y,
										id_T++		)
		{
			sphs1 = texelFetch(Content,ivec2(i,0),0);
			if(isSphereIntersect(me,lookTo,sphs1.yzw,sphs1.x,distan1,distan2))
			{
				lenNow = min(distan1,distan2);
				if(lenNow < len)
				{
					len = lenNow;
					id = id_T;
				}
			}
		}
		if (len==inf) return false;
		return true;
	}
функции текстурирования и растекстурирования от номера элемента
соответственно как 

				else if(id.x==3)
				{
					pointInTex = triToTex2(sphs1.xyz,vec3(sphs1.w,sphs2.xy),vec3(sphs2.zw,sphs3.x),me);
					vecInBasis = VtT(lookTo,vec3(sphs1.w,sphs2.xy)-sphs1.xyz,norm);
				}
				
и

				if(data1.x==1)
				{
					me = textoPSph2(dd1.yzw,dd1.x*1.001,dd2.xyz,vec3(dd2.w,dd3.xy),pointInTex);
					norm = normalize(me-dd1.yzw);
					lookTo = StV(vecInBasis,dd2.xyz,vec3(dd2.w,dd3.xy),norm);
				}

функцию получения нормали от данных элемента и оформить например

		 if(id.x==/*texelFetch(ContentAbout,ivec2(0,0),0).w*/1)//если сфера
		{
			norm = sphN(sphs1.yzw,me,lookTo,leng.x);
		}
		
условие для объекта типа:+-425 строка

	leng.y = inf;

	id_T.x=texelFetch(ContentAbout,ivec2(2,0),0).w;
		if(getBoxClr(me,lookTo,texelFetch(ContentAbout,ivec2(2,0),0),texelFetch(ContentAbout,ivec2(3,0),0),leng.y,id_T.y))
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y;
			id= id_T;
			}

		}
также надо передать в текстуру данные об объекте
например, так

		{
			hans.clear();
			hans2.clear();
			hans3.clear();
			for (auto& j : vec2)
			{
				const auto& q = j.get();
				for (auto z : q)hans.push_back(z);
				hans2.push_back(j.materialID);
				hans3.push_back(j.materialClass);
			}
			tx.addData(Box::size, vec2.size(), 2, &hans[0]);
			tx.addMaterials(matBeg, &hans2[0], &hans3[0]);
		}
		
где vec2 это вектор с объектами, у которых есть методы getData и поля materialID и materialClass которые соответственно являются ид материала(нумерация с 0) и его классом также есть поле size которое отвечает за размер данных, округленный до кратного 4
для добавления материала нужно сделать почти все то же самое,
только в код добавить что-то подобное

			elseif(in_T.w==/*matClass*/)
			{
				//реализация
				//на выходе цвет в переменной color_t
				
			} и в с++ то же самое что и для объекта
