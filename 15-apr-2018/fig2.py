xs1 = {"x{}".format(i):np.linspace(-i,i,100) for i in range(50)}
ps1 = 1/100
means = 0
f1 = lambda x: ps1*(x-means)**2
variances1 = [f1(xs1["x{}".format(i)]).sum() for i in range(50)]

xs2 = {"x{}".format(i):np.linspace(-10, 10, 2 * i) for i in range(1,50)}
f2 = lambda x: (x-means)**2
variances2 = [f2(xs2["x{}".format(i)]).sum()/(2*i) for i in range(1, 50)]

plt.subplot(211); plt.plot(range(50), variances1)
plt.title("Variance of distribution of random uniform discrete variable of range (-x, x)")
plt.xlabel("x");plt.ylabel("variance");
plt.subplot(212); plt.plot(range(1, 50), variances2)
plt.savefig("fig2.png")
plt.show()