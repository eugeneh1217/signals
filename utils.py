import pandas as pd
import plotly.express as px

df = pd.read_csv("signaltap.csv")

diff = df.diff()

print(
    f"time:\n"
    f"\tmin, max: {diff['t'].min()}, {diff['t'].max()}\n"
    f"\tavg: {diff['t'].mean()}\n"
    f"\tmed: {diff['t'].median()}\n"
    f"\tstd: {diff['t'].std()}\n"
)

fig = px.scatter(df, x='t', y='c0', labels={'t': 'Time (ms)', 'c0': 'Signal'})

fig.show()

# ten = pd.read_csv("10ms.csv")
# hun = pd.read_csv("100ms.csv")

# ten = ten.diff()
# hun = hun.diff()

# print(ten.info())

# print(
#     f"time:\n"
#     f"\tmin, max: {ten['t'].min()}, {ten['t'].max()}\n"
#     f"\tavg: {ten['t'].mean()}\n"
#     f"\tmed: {ten['t'].median()}\n"
#     f"\tstd: {ten['t'].std()}\n"
# )

# print(
#     f"time:\n"
#     f"\tmin, max: {hun['t'].min()}, {hun['t'].max()}\n"
#     f"\tavg: {hun['t'].mean()}\n"
#     f"\tmed: {hun['t'].median()}\n"
#     f"\tstd: {hun['t'].std()}\n"
# )
