function [w1, y,S]= HebbNN(x,t,w_init)
m=size(x,1)%number of neutrons
n=size(x,2)  %number of training samples (rows)
w=zeros(1,n) ; %initilization of weights
y=zeros(1,m);
s=0;
S=zeros(1,m);
w0=0;
while isequal(y,t)==0
for i=1:m
              for j=1:n
                  if x(i,j)*t(i)==1 
                      delta=1;
                  elseif x(i,j)==0
                        delta=0;
                  elseif x(i,j)~=0 && t(i)==0
                      delta=-1;
                  end
                w(j)=w(j)+delta;
              end 
           y
   w
    W(i,:)=w;  
    w1=W;
end
y=y;
for i=1:m
    a=0;
    %w0=1;
 for j=1:n
       a=a+x(i,j)*W(m,j);
     end
        s(i)=a+w_init;
           if s(i)>1
              y(i)=1;
           else 
             y(i)=0;
           end
end
S=s;
end
end