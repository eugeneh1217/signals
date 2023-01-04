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
