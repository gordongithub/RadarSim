%UKF (�޼��������˲�����

function [X_eNew,P_New]=UKF_Origin1(T,Q,R,X_e,Z,Ownship,P)

%����ֵ
L=9;      %ά��
alpha=1e-4;
kappa=0;
beta=2;
lambda=alpha^2*(L+kappa)-L;

w(1,1)=lambda/(L+lambda);
v(1,1)=lambda/(L+lambda)+1-alpha^2+beta;
for j=2:(2*L+1)
    w(1,j) = 1 / ( 2 * ( L + lambda));
    v(1,j) = 1 / ( 2 * ( L + lambda));
end

%״̬ת�ƾ���
F=[1 T 0.5*T^2 0 0 0 0 0 0;
   0 1 T 0 0 0 0 0 0;
   0 0 1 0 0 0 0 0 0;
   0 0 0 1 T 0.5*T^2 0 0 0;
   0 0 0 0 1 T 0 0 0;
   0 0 0 0 0 1 0 0 0;
   0 0 0 0 0 0 1 T 0.5*T^2;
   0 0 0 0 0 0 0 1 T;
   0 0 0 0 0 0 0 0 1];
G=[0.5*T^2 0 0;
    T 0 0;
    1 0 0;
    0 0.5*T^2 0;
    0 T 0;
    0 1 0;
    0 0 0.5*T^2;
    0 0 T;
    0 0 1];
% Q=eye(3)*Q;
%�˲�
    X_extend=X_e(:,end);
    P_extend=P(:,:,end);
    
    %����sigma��
    P_sqrt=schol(P_extend);
    X_sigma(:,1)=X_extend;
    for j=1:L
        X_sigma(:,j+1)=X_extend+(L+lambda)^0.5*P_sqrt(:,j);
        X_sigma(:,j+L+1)=X_extend-(L+lambda)^0.5*P_sqrt(:,j);
    end
    %����ת�ƺ��sigma��
    for j=1:(2*L+1)
        Xi_sigma(:,j)=F*X_sigma(1:9,j);
    end
    %����״̬�������ֵ
    X_eNew(:,1)=Xi_sigma*w';
    
    %����״̬Ԥ��Э����
    P_x=Q;
    for j=1:(2*L+1)
        P_x=P_x+v(1,j)*(Xi_sigma(:,j)-X_eNew(:,1))*(Xi_sigma(:,j)-X_eNew(:,1))';
    end
    
        xo1=Ownship(1).P(1,end);
        yo1=Ownship(1).P(4,end);
        zo1=Ownship(1).P(7,end);
        xo2=Ownship(2).P(1,end);
        yo2=Ownship(2).P(4,end);
        zo2=Ownship(2).P(7,end);
        xo3=Ownship(3).P(1,end);
        yo3=Ownship(3).P(4,end);
        zo3=Ownship(3).P(7,end);
    
    %����۲��sigma��
    for j=1:(2*L+1)
        x=Xi_sigma(1,j);y=Xi_sigma(4,j);z=Xi_sigma(7,j);
        Z_sigma(:,j)=[atan((y-yo1)/(x-xo1));atan((z-zo1)/sqrt(((x-xo1)^2+(y-yo1)^2)));atan((y-yo2)/(x-xo2));atan((z-zo2)/sqrt(((x-xo2)^2+(y-yo2)^2)));atan((y-yo3)/(x-xo3));atan((z-zo3)/sqrt(((x-xo3)^2+(y-yo3)^2)))];%�˴����޸�
    end  
   %����۲����ֵ
   Z_eNew(:,1)=Z_sigma*w';
   
   %����۲�ķ���
   P_z=R;
   for j=1:(2*L+1)
       P_z=P_z+v(1,j)*(Z_sigma(:,j)-Z_eNew(:,1))*(Z_sigma(:,j)-Z_eNew(:,1))';
   end
   
   %����X��Z�Ľ��淽��
   P_xz=zeros(9,6);
   for j=1:(2*L+1)
       P_xz=P_xz+v(1,j)*(Xi_sigma(:,j)-X_eNew(:,1))*(Z_sigma(:,j)-Z_eNew(:,1))';
   end
   %�������£�������
   Residual=Z(:,end)-Z_eNew(:,1);
   K=P_xz*inv(P_z);
   X_eNew(:,1)=X_eNew(:,1)+K*Residual;
   P_New=P_x-K*P_z*K';
end