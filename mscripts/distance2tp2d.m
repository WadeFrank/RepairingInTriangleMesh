function [d]=distance2tp2d(X,tp)
%�㵽������Ƭ�ľ���
% X m-by-3
% tp 3-by-3,ÿһ�б�ʾһ����������

A=tp(1,:);
B=tp(2,:);
C=tp(3,:);

AB=B-A;
BC=C-B;
n=cross(AB,BC);
AX=X-A;
dn=AX*n'./norm(n);  %0

%���X��������Ƭ�ϵ�ͶӰXt
Xt=A+AX-dn*n/norm(n); %==X


[r,u,v]=point_in_triangle(Xt,A,B,C);
AXt=Xt-A;
BXt=Xt-B;
pi_2=pi/2;
angleBCXt=point3theta(B,C,Xt);
angleCBXt=point3theta(C,B,Xt);
angleACXt=point3theta(A,C,Xt);
angleCAXt=point3theta(C,A,Xt);
angleABXt=point3theta(A,B,Xt);
angleBAXt=point3theta(B,A,Xt);
dAB=norm(BXt)*sin(angleABXt);
dBC=norm(BXt)*sin(angleCBXt);
dCA=norm(AXt)*sin(angleCAXt);
dA=norm(Xt-A);
dB=norm(Xt-B);
dC=norm(Xt-C);      

if r
    sdn=1;
else
    sdn=-1;
end
d=0;
if ~r
    %���Xt����������Ƭ��
    if u>0 && v>0 && u+v>1
        if angleBCXt < pi_2 && angleCBXt < pi_2
            d=dBC;
        elseif angleBCXt < pi_2 && angleCBXt > pi_2
            d=dB;
        elseif angleBCXt > pi_2 && angleCBXt < pi_2
            d=dC;
        end
    elseif u>0 && v<0 && u+v<1            
        if angleACXt < pi_2 && angleCAXt < pi_2
            d=dCA;
        elseif angleACXt < pi_2 && angleCAXt > pi_2
            d=dA;
        elseif angleACXt > pi_2 && angleCAXt < pi_2
            d=dC;   
        end        
    elseif u<0 && v>0 && u+v<1         
        if angleABXt < pi_2 && angleBAXt < pi_2
            d=dAB;
        elseif angleABXt < pi_2 && angleBAXt > pi_2
            d=dA;
        elseif angleABXt > pi_2 && angleBAXt < pi_2
            d=dB;       
        end
    elseif u<0 && v<0 && u+v<1
        d=dA;
    elseif u<0 && v>0 && u+v>1
        d=dB;
    elseif u>0 && v<0 && u+v>1
        d=dC;
    end
else    
    %���Xt��������Ƭ��
    d=min([dAB,dBC,dCA]);
end
d=sdn*d;
end