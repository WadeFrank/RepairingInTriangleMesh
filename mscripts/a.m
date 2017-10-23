%%
clear
clc
load vertex.mat
x = vertexes(:,1);
y = vertexes(:,2);
z = vertexes(:,3);
tn = length(x) / 3;

%����������Ƭ������
normals = [];
for i=0:1:tn-1
    ab = [x(i*3+1) - x(i*3+2) 
        y(i*3+1) - y(i*3+2) 
        z(i*3+1) - z(i*3+2)
        ];
    bc = [x(i*3+2) - x(i*3+3) 
        y(i*3+2) - y(i*3+3) 
        z(i*3+2) - z(i*3+3)
        ];
    normal = cross(ab,bc)';
    normal = normal / norm(normal);
    if normal(3) < 0
        normal = -normal;
    end
    normals = [normals; normal];
end

figure;
scatter3(x, y, z, '*');
figure;
scatter3(normals(:,1), normals(:,2), normals(:,3), 'o');

%%
% �������
%k-means����
data = normals;
[u re]=KMeans(data,5);  
[m n]=size(re);

%�����ʾ����������
figure;
hold on;
for i=1:m 
    if re(i,4)==1   
         plot3(re(i,1),re(i,2),re(i,3),'ro'); 
    elseif re(i,4)==2
         plot3(re(i,1),re(i,2),re(i,3),'go'); 
    elseif re(i,4)==3
         plot3(re(i,1),re(i,2),re(i,3),'bo'); 
    elseif re(i,4)==4
         plot3(re(i,1),re(i,2),re(i,3),'ys'); 
    else 
         plot3(re(i,1),re(i,2),re(i,3),'ms'); 
    end
end
grid on;
