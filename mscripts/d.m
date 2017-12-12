normals=zeros(tn2,3);
for i=1:tn2
    vs=[vertex_set(tpis(i,1),:);
       vertex_set(tpis(i,2),:);
       vertex_set(tpis(i,3),:)];
    
    ab = vs(1,:) - vs(2,:);
    bc = vs(2,:) - vs(3,:);
    normal = cross(ab,bc)';
    normals(i,:)=normal/norm(normal);
end

%%
% �������
%k-means����
normals_m=normals(model_tp_k,:);
data = normals_m;
[u re]=KMeans(data,5);  
[m n]=size(re);

%�����ʾ�����ķ���������
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
         plot3(re(i,1),re(i,2),re(i,3),'yo'); 
    else 
         plot3(re(i,1),re(i,2),re(i,3),'ms'); 
    end
end
grid on;